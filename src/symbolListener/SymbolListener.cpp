/*  SPL - Simple Programming Language compiler
 *  Copyright (C) 2022  Valentyn Tymchyshyn
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or any
 *  later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 *  Valentyn Tymchyshyn (val.t.develop) (val.t.develo@gmail.com)
 *
 *  Step of compilation where all symbols (variables, methods, classes) in code are remembered.
 */

#include "SymbolListener.hpp"
#include <main.hpp>
#include <utils/Out.hpp>
#include <utils/ParserUtils.hpp>
#include <utils/UniqueNumber.hpp>

SymbolListener::SymbolListener(shared_ptr<SymbolTable> symbolTable,
                               Path filePath, Lexer lexer)
    : lexer(lexer) {
    this->symbolTable = symbolTable;
    this->filePath = filePath;
}

void SymbolListener::processImport(vector<string> importName) {
    vector<Path> toImport = vector<Path>();
    for (auto src : ArgsParser::src) {
        if (src.isFile()) {
            processImportFile(src, toImport, importName);
        } else if (src.isDir()) {
            processImportDir(src, toImport, importName);
        }
    }

    if (toImport.empty()) {
        Out::errorMessage(lexer, "Can not find import");
    } else {
        for (auto p : toImport) {
            Main::processFileToState(p, CU::State::ST);
            symbolTable->addImport(Main::CUs[p]->st);
        }
        Main::currCUsStack.top()->importFiles[importName] = toImport;
    }
}

void SymbolListener::processImportFile(Path& src, vector<Path>& toImport, vector<string> importName) {
    vector<string> vec = split(src.getParent().getName(), "/");
    for (int i = 0; i < importName.size(); ++i) {
        if (importName[importName.size()-1-i]!=vec[vec.size()-1-i]) {
            return;
        }
    }
    if (src.getFilename().ends_with(".spl")) {
        if (src.getName()==filePath.getName()) {
            return;
        }
        for (auto el : toImport) {
            if (el.getName()==src.getName()) {
                return;
            }
        }
        toImport.push_back(src);
    }
}

void SymbolListener::processImportDir(Path& dir, vector<Path>& toImport, vector<string> importName) {
    for (auto src : dir.getDirContent()) {
        if (src.isFile()) {
            processImportFile(src, toImport, importName);
        } else if (src.isDir()) {
            processImportDir(src, toImport, importName);
        }
    }
}

void SymbolListener::walk() {
    symbolTable->setCurrentScopeNameAndType("__program", "__program");
    ParserUtils::skipSemicolons(lexer);
    if (lexer.getCurrent()->kind == Token::Kind::PACKAGE) {
        enterPackage();
    }
    while (true) {
        ParserUtils::skipSemicolons(lexer);
        if (lexer.getCurrent()->kind == Token::Kind::IMPORT) {
            enterImport();
        } else {
            break;
        }
    }

    while (lexer.getCurrent()->kind != Token::Kind::END_OF_FILE) {
        ParserUtils::skipSemicolons(lexer);
        enterTypeDecl();
    }
}

void SymbolListener::enterPackage() {
    if (lexer.getCurrent()->kind == Token::Kind::PACKAGE) {
        lexer.goForward();
        ParserUtils::QualifiedName qualifiedName(lexer);

        Path path = filePath.getParent();
        for (int i = qualifiedName.size() - 1; i >= 0; i--) {
            string name = qualifiedName.get(i)->str;
            if (path.getName().ends_with(name)) {
                path = path.getParent();
            } else {
                Out::errorMessage(lexer, "Irregular package name");
            }
        }
        srcDir = path;

        vector<string> importName = {"spl", "core"};
        processImport(importName);
    }
}

void SymbolListener::enterImport() {
    if (lexer.getCurrent()->kind == Token::Kind::IMPORT) {
        lexer.goForward();
        ParserUtils::QualifiedName qualifiedName(lexer);

        vector<string> importName;
        std::stringstream streamData(qualifiedName.getText());
        std::string val;
        while (std::getline(streamData, val, '.')) {
            importName.push_back(val);
        }

        processImport(importName);
    }
}

void SymbolListener::enterTypeDecl() {
    enterModifiers();
    if (lexer.getCurrent()->kind == Token::Kind::CLASS) {

        enterClassDecl();
    } else if (lexer.getCurrent()->kind == Token::Kind::INTERFACE) {
        enterInterfaceDecl();
    } else if (lexer.getCurrent()->kind == Token::Kind::ENUM) {
        enterEnumDecl();
    } else {
        Out::errorMessage(lexer,
                          "Expected type declaration kind specifier (class, "
                          "interface, enum or @interface), but found:\n\t" +
                              lexer.getCurrent()->str + "\tin " +
                              std::to_string(lexer.getCurrent()->line) + ":" +
                              std::to_string(lexer.getCurrent()->pos));
    }
}

void SymbolListener::enterClassDecl() {
    string type = "", id = "";

    lexer.goForward();
    if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER) {
        id = lexer.getCurrent()->str;
        lexer.goForward();

        if (symbolTable->lookupClass(id) != nullptr) {
            Out::errorMessage(
                lexer, "Duplicated class name [ " + id + " ] in " +
                           std::to_string(lexer.getCurrent()->line) + ":" +
                           std::to_string(lexer.getCurrent()->pos));
        }
    } else {
        Out::errorMessage(lexer, "Expected identifier, but found:\n\t" +
                                     lexer.getCurrent()->str + "\tin " +
                                     std::to_string(lexer.getCurrent()->line) +
                                     ":" +
                                     std::to_string(lexer.getCurrent()->pos));
    }

    shared_ptr<ClassRecord> newClass = make_shared<ClassRecord>(id, type);
    if (currentClass != nullptr) {
        currentClass->addInnerClass(newClass);
    }
    currentClass = newClass;

    symbolTable->put(newClass);
    // enter class SCOPE
    symbolTable->enterScope(newClass);
    // set scope name & type
    symbolTable->setCurrentScopeNameAndType(id, "__class");
    symbolTable->setCurrentScopeClass(newClass);
    classes.push(currentClass);

    if (lexer.ifCurrTokenStartsWithLT()) {
        type = enterGenericDecl();
        currentClass->type = type;
    }

    vector<string> superClasses = enterExtending();
    newClass->superClass = superClasses.size() >= 1 ? superClasses[0] : "";

    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        lexer.goForward();
        while (true) {
            ParserUtils::skipSemicolons(lexer);
            if (lexer.getCurrent()->kind == Token::Kind::RBRACE) {
                lexer.goForward();
                break;
            }
            enterClassMemberDecl();
        }
    } else {
        Out::errorMessage(
            lexer, "Expected '{', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    symbolTable->exitScope();
    classes.pop();
    if (!classes.empty()) {
        currentClass = classes.top();
    } else {
        currentClass = nullptr;
    }
}

void SymbolListener::enterInterfaceDecl() { enterClassDecl(); }

void SymbolListener::enterEnumDecl() { enterClassDecl(); }

string SymbolListener::enterGenericDecl() {
    string str = "<";
    lexer.removeFirstCharOfCurrShift();

    while (true) {
        if (lexer.ifCurrTokenStartsWithGT()) {
            str.append(">");
            lexer.removeFirstCharOfCurrShift();
            break;
        }
        str.append(enterGenericTypeDecl());
        if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
            str.append(lexer.getCurrent()->str);
            lexer.goForward();
        }
    }
    return str;
}

string SymbolListener::enterGenericTypeDecl() {
    string str = "";
    if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER) {
        string id = lexer.getCurrent()->str;
        str.append(id);
        lexer.goForward();
        // TODO <T extends A>

        shared_ptr<ClassRecord> newClass = make_shared<ClassRecord>(id, "");
        if (currentClass != nullptr) {
            currentClass->addInnerClass(newClass);
        }
        symbolTable->put(newClass);
    } else {
        Out::errorMessage(lexer, "Expected identifier, but found:\n\t" +
                                     lexer.getCurrent()->str + "\tin " +
                                     std::to_string(lexer.getCurrent()->line) +
                                     ":" +
                                     std::to_string(lexer.getCurrent()->pos));
    }
    return str;
}

void SymbolListener::enterClassMemberDecl() {
    vector<ModifiersNode::ModifierKind> mods = enterModifiers();
    if (lexer.getCurrent()->kind == Token::Kind::CLASS) {
        enterClassDecl();
    } else if (lexer.getCurrent()->kind == Token::Kind::INTERFACE) {
        enterInterfaceDecl();
    } else if (lexer.getCurrent()->kind == Token::Kind::ENUM) {
        enterEnumDecl();
    } else {
        if (lexer.getCurrent()->kind==Token::Kind::TILDE) {
            enterDestructorDecl(mods);
        } else if (lexer.getNext()->kind == Token::Kind::LPAREN) {
            enterConstructorDecl(mods);
        } else {
            string type, id = "";
            if (lexer.getCurrent()->kind == Token::Kind::VOID) {
                type = "void";
                lexer.goForward();
            } else {
                type = enterType(true);
            }

            if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER) {
                id = lexer.getCurrent()->str;
                lexer.goForward();
                if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
                    enterMethodDecl(type, id, mods);
                } else {
                    if (type == "void") {
                        Out::errorMessage(
                            lexer,
                            "Field type can not be void in " +
                                std::to_string(lexer.getPrevious()->line) +
                                ":" + std::to_string(lexer.getPrevious()->pos));
                    } else {
                        enterField(type, id, mods);
                    }
                }
            } else {
                Out::errorMessage(
                    lexer, "Expected identifier, but found:\n\t" +
                               lexer.getCurrent()->str + "\tin " +
                               std::to_string(lexer.getCurrent()->line) + ":" +
                               std::to_string(lexer.getCurrent()->pos));
            }
        }
    }
}

void SymbolListener::enterDestructorDecl(
    vector<ModifiersNode::ModifierKind> mods) {
    lexer.goForward();
    if (lexer.getCurrent()->str != currentClass->id) {
        Out::errorMessage(lexer, "Destructor has not same name as class [ " +
                                     currentClass->id + " ] in " +
                                     std::to_string(lexer.getCurrent()->line) +
                                     ":" +
                                     std::to_string(lexer.getCurrent()->pos));
    }
    string type = "__destructor", id = "__spl__destructor__"+lexer.getCurrent()->str;

    currentMethod = make_shared<MethodRecord>(id, type);
    currentMethod->isDestructor = true;
    currentMethod->mods = mods;
    symbolTable->put(currentMethod);

    // enter METHOD SCOPE
    symbolTable->enterScope(currentMethod);
    // set scope name
    symbolTable->setCurrentScopeNameAndType(id, "__destructor");
    // add method to currentClass
    currentClass->addMethod(currentMethod);
    // inherit current class from parent scope
    symbolTable->setCurrentScopeClass(currentClass);

    lexer.goForward();
    if (lexer.getCurrent()->kind == Token::Kind::LPAREN && lexer.getNext()->kind == Token::Kind::RPAREN) {
        lexer.goForward();
        lexer.goForward();
    }

    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        enterBlockStatement(false);
    } else if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected '{', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    uint8_t destructors = 0;
    for (shared_ptr<MethodRecord> method : currentClass->methods) {
        if (method->isDestructor == true) {
            destructors++;
        }
    }
    if (destructors >= 2) {
        Out::errorMessage(lexer, "Destructor duplicated on class [ " +
                                     currentClass->id + " ]");
    }
    symbolTable->exitScope();
}


void SymbolListener::enterConstructorDecl(
    vector<ModifiersNode::ModifierKind> mods) {
    if (lexer.getCurrent()->str != currentClass->id) {
        Out::errorMessage(lexer, "Constructor has not same name as class [ " +
                                     currentClass->id + " ] in " +
                                     std::to_string(lexer.getCurrent()->line) +
                                     ":" +
                                     std::to_string(lexer.getCurrent()->pos));
    }
    string type = "__constructor", id = lexer.getCurrent()->str;

    currentMethod = make_shared<MethodRecord>(id, type);
    currentMethod->isConstructor = true;
    currentMethod->mods = mods;
    symbolTable->put(currentMethod);

    // enter METHOD SCOPE
    symbolTable->enterScope(currentMethod);
    // set scope name
    symbolTable->setCurrentScopeNameAndType(id, "__constructor");
    // add method to currentClass
    currentClass->addMethod(currentMethod);
    // inherit current class from parent scope
    symbolTable->setCurrentScopeClass(currentClass);

    lexer.goForward();
    enterMethodArgs();

    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        enterBlockStatement(false);
    } else if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected '{', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    uint8_t similarities = 0;
    for (shared_ptr<MethodRecord> method : currentClass->methods) {
        if (method->id == currentMethod->id) {
            if (method->vars == currentMethod->vars) {
                similarities++;
            }
        }
    }
    if (similarities >= 2) {
        Out::errorMessage(lexer, "Constructor duplicated on class [ " +
                                     currentClass->id + " ]");
    }
    symbolTable->exitScope();
}

void SymbolListener::enterMethodDecl(string type, string id,
                                     vector<ModifiersNode::ModifierKind> mods) {
    currentMethod = make_shared<MethodRecord>(id, type);
    currentMethod->similar.push_back(currentMethod);
    currentMethod->mods = mods;
    symbolTable->put(currentMethod);

    // enter METHOD SCOPE
    symbolTable->enterScope(currentMethod);
    // set scope name
    symbolTable->setCurrentScopeNameAndType(id, "__method");
    // add method to currentClass
    currentClass->addMethod(currentMethod);
    // inherit current class from parent scope
    symbolTable->setCurrentScopeClass(currentClass);
    bool staticMod = false;
    for (auto mod : mods) {
        if (mod == ModifiersNode::ModifierKind::STATIC) {
            staticMod = true;
        }
    }

    if (!staticMod) {
        shared_ptr<VarRecord> thisVar = make_shared<VarRecord>(
                    "this", currentClass->id, Record::RecordKind::LOCAL_VAR_RECORD);
        // insert record into scope
        currentMethod->addArg(thisVar);
        // insert record into scope
        symbolTable->put(thisVar);
        thisVar->ir_name="this";
        thisVar->typeRec=currentClass;
    }

    enterMethodArgs();

    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        enterBlockStatement(false);
    } else if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected '{', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    uint8_t similarities = 0;
    for (shared_ptr<MethodRecord> method : currentClass->methods) {
        if (method->equals(currentMethod)) {
            similarities++;
        }
    }
    if (similarities >= 2) {
        Out::errorMessage(lexer, "Method [ " + currentMethod->id +
                                     " ] duplicated on class [ " +
                                     currentClass->id + " ]");
    }
    symbolTable->exitScope();
}

void SymbolListener::enterField(string type, string id,
                                vector<ModifiersNode::ModifierKind> mods) {
    int dims = 0;

    while (true) {
        if (lexer.getCurrent()->kind == Token::Kind::LBRACKET) {
            lexer.goForward();
            dims++;
            if (lexer.getCurrent()->kind == Token::Kind::RBRACKET) {
                lexer.goForward();
            } else {
                Out::errorMessage(
                    lexer, "Expected ']', but found:\n\t" +
                               lexer.getCurrent()->str + "\tin " +
                               std::to_string(lexer.getCurrent()->line) + ":" +
                               std::to_string(lexer.getCurrent()->pos));
            }
        } else {
            break;
        }
    }

    for (int i = 0; i < dims; ++i) {
        type += "[]";
    }

    shared_ptr<VarRecord> newField =
        make_shared<VarRecord>(id, type, Record::RecordKind::FIELD_RECORD);
    newField->mods = mods;
    newField->next = currentClass;
    // insert record into scope
    currentClass->addField(newField);
    symbolTable->setCurrentScopeClass(currentClass);
    // insert record into scope
    symbolTable->put(newField);

    if (lexer.getCurrent()->kind == Token::Kind::ASSIGN) {
        lexer.goForward();
        enterExpression();
    }
}

void SymbolListener::enterStatement() {
    vector<ModifiersNode::ModifierKind> mods = enterModifiers();
    if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER ||
        lexer.getCurrent()->kind == Token::Kind::BOOL ||
        lexer.getCurrent()->kind == Token::Kind::CHAR ||
        lexer.getCurrent()->kind == Token::Kind::BYTE ||
        lexer.getCurrent()->kind == Token::Kind::SHORT ||
        lexer.getCurrent()->kind == Token::Kind::INT ||
        lexer.getCurrent()->kind == Token::Kind::LONG ||
        lexer.getCurrent()->kind == Token::Kind::FLOAT ||
        lexer.getCurrent()->kind == Token::Kind::DOUBLE) {
        int i = 1;
        while (true) {
            if (lexer.getWithOffset(i)->kind == Token::Kind::IDENTIFIER) {
                enterLocalVar(mods);
                break;
            } else if (lexer.getWithOffset(i)->kind == Token::Kind::LBRACKET ||
                       lexer.getWithOffset(i)->kind == Token::Kind::RBRACKET) {
                i++;
            } else {
                enterNotVarStatement();
                break;
            }
        }
    } else {
        enterNotVarStatement();
    }
}

void SymbolListener::enterNotVarStatement() {
    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        enterBlockStatement(true);
    } else if (lexer.getCurrent()->kind == Token::Kind::ASSERT) {
        enterAssertStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::BREAK) {
        enterBreakStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::CONTINUE) {
        enterContinueStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::RETURN) {
        enterReturnStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::IF) {
        enterIfStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::WHILE) {
        enterWhileStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::FOR) {
        enterForStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        enterExpression();
    }
}

void SymbolListener::enterLocalVar(vector<ModifiersNode::ModifierKind> mods) {
    string type = enterType(true), id = "";

    while (true) {
        if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER) {
            id = lexer.getCurrent()->str;
            lexer.goForward();

            int dims = 0;

            while (true) {
                if (lexer.getCurrent()->kind == Token::Kind::LBRACKET) {
                    lexer.goForward();
                    dims++;
                    if (lexer.getCurrent()->kind == Token::Kind::RBRACKET) {
                        lexer.goForward();
                    } else {
                        Out::errorMessage(
                            lexer,
                            "Expected ']', but found:\n\t" +
                                lexer.getCurrent()->str + "\tin " +
                                std::to_string(lexer.getCurrent()->line) + ":" +
                                std::to_string(lexer.getCurrent()->pos));
                    }
                } else {
                    break;
                }
            }

            for (int i = 0; i < dims; ++i) {
                type += "[]";
            }

            shared_ptr<VarRecord> newVar = make_shared<VarRecord>(
                id, type, Record::RecordKind::LOCAL_VAR_RECORD);
            newVar->mods = mods;
            // insert record into scope
            currentMethod->addVar(newVar);
            // insert record into scope
            symbolTable->put(newVar);

            if (lexer.getCurrent()->kind == Token::Kind::ASSIGN) {
                lexer.goForward();
                enterExpression();
            }

            if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                lexer.goForward();
            } else {
                break;
            }

        } else {
            Out::errorMessage(
                lexer, "Expected identifier, but found:\n\t" +
                           lexer.getCurrent()->str + "\tin " +
                           std::to_string(lexer.getCurrent()->line) + ":" +
                           std::to_string(lexer.getCurrent()->pos));
        }
    }
}

void SymbolListener::enterMethodArgs() {
    lexer.goForward();
    while (true) {
        if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
            lexer.goForward();
            break;
        } else if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
            lexer.goForward();
        }
        string type = enterType(true), id = "";
        if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER) {
            id = lexer.getCurrent()->str;
            lexer.goForward();

            int dims = 0;

            while (true) {
                if (lexer.getCurrent()->kind == Token::Kind::LBRACKET) {
                    lexer.goForward();
                    dims++;
                    if (lexer.getCurrent()->kind == Token::Kind::RBRACKET) {
                        lexer.goForward();
                    } else {
                        Out::errorMessage(
                            lexer,
                            "Expected ']', but found:\n\t" +
                                lexer.getCurrent()->str + "\tin " +
                                std::to_string(lexer.getCurrent()->line) + ":" +
                                std::to_string(lexer.getCurrent()->pos));
                    }
                } else {
                    break;
                }
            }

            for (int i = 0; i < dims; ++i) {
                type += "[]";
            }
        } else {
            Out::errorMessage(
                lexer, "Expected identifier, but found:\n\t" +
                           lexer.getCurrent()->str + "\tin " +
                           std::to_string(lexer.getCurrent()->line) + ":" +
                           std::to_string(lexer.getCurrent()->pos));
        }

        shared_ptr<VarRecord> arg = make_shared<VarRecord>(
            id, type, Record::RecordKind::LOCAL_VAR_RECORD);
        // add parameter to method
        currentMethod->addArg(arg);
        // insert record into scope
        symbolTable->put(arg);
    }
}

void SymbolListener::enterBlockStatement(bool newScope) {
    if (newScope) {
        string id = "__jpp__" + std::to_string(getNextUniqueNumber());
        shared_ptr<Record> record =
            make_shared<Record>(id, "__jpp__group", Record::RecordKind::UNUSED);
        symbolTable->put(record);
        symbolTable->enterScope(record);
    }

    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        lexer.goForward();
        while (true) {
            ParserUtils::skipSemicolons(lexer);
            if (lexer.getCurrent()->kind == Token::Kind::RBRACE) {
                if (newScope) {
                    symbolTable->exitScope();
                }
                lexer.goForward();
                break;
            }
            enterStatement();
        }
    } else {
        Out::errorMessage(
            lexer, "Expected '{', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
}

void SymbolListener::enterAssertStatement() {
    lexer.goForward();
    enterExpression();
    if (lexer.getCurrent()->kind == Token::Kind::COLON) {
        lexer.goForward();
        enterExpression();
    }

    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
}

void SymbolListener::enterBreakStatement() {
    lexer.goForward();
    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
}

void SymbolListener::enterContinueStatement() {
    lexer.goForward();
    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
}

void SymbolListener::enterReturnStatement() {
    lexer.goForward();

    if (lexer.getCurrent()->kind != Token::Kind::SEMICOLON) {
        enterExpression();
    }

    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
}

void SymbolListener::enterIfStatement() {
    lexer.goForward();

    if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected '(', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    enterExpression();

    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ')', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    enterStatement();

    if (lexer.getCurrent()->kind == Token::Kind::ELSE) {
        lexer.goForward();
        enterStatement();
    }
}

void SymbolListener::enterWhileStatement() {
    lexer.goForward();

    if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected '(', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    enterExpression();

    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ')', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    enterStatement();
}

void SymbolListener::enterForStatement() {
    lexer.goForward();

    if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected '(', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    enterStatement();
    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    enterExpression();
    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    if (lexer.getCurrent()->kind != Token::Kind::RPAREN) {
        enterStatement();
    }

    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ')', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    enterStatement();
}

void SymbolListener::enterForEachStatement() {
    lexer.goForward();

    if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected '(', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    std::vector<ModifiersNode::ModifierKind> mods = enterModifiers();
    enterLocalVar(mods);

    if (lexer.getCurrent()->kind == Token::Kind::COLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ':', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    enterExpression();

    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ')', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    enterStatement();
}

void SymbolListener::enterExpression() {
    enterUnOpPrimary();
    enterBinOpRHS(0);
}

void SymbolListener::enterBinOpRHS(int exprPrec) {
    while (true) {
        int tokPrec = ParserUtils::getBinOpPrecedence(lexer);

        if (tokPrec < exprPrec)
            return;

        string binOp = lexer.getCurrent()->str;
        lexer.goForward();

        enterUnOpPrimary();

        int nextPrec = ParserUtils::getBinOpPrecedence(lexer);
        if (tokPrec < nextPrec) {
            enterBinOpRHS(tokPrec + 1);
        }
    }
}

void SymbolListener::enterUnOpPrimary() {
    while (true) {
        if (lexer.getCurrent()->kind == Token::Kind::INC) {
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::DEC) {
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::BANG) {
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::TILDE) {
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::ADD) {
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::SUB) {
            lexer.goForward();
        } else {
            break;
        }
    }

    enterPrimary();

    while (true) {
        if (lexer.getCurrent()->kind == Token::Kind::INC) {
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::DEC) {
            lexer.goForward();
        } else {
            break;
        }
    }
}

void SymbolListener::enterPrimary() {
    if (lexer.getCurrent()->kind == Token::Kind::BOOL_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::CHAR_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::STRING_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_FLOAT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_FLOAT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_DOUBLE_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_DOUBLE_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_BYTE_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_BYTE_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_SHORT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_SHORT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_INT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_INT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_LONG_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_LONG_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::NULL_LITERAL) {
        enterLiteral();
    } else if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        enterParenExpr();
    } else if (lexer.getCurrent()->kind == Token::Kind::NEW) {
        enterNew();
    } else if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        enterArrayInitializer();
    } else {
        enterAccessOrCall(true);
    }
}

void SymbolListener::enterNew() {
    lexer.goForward();

    if (lexer.getCurrent()->kind == Token::Kind::STATIC) {
        lexer.goForward();
    }

    enterType(false);

    if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        lexer.goForward();

        while (true) {
            if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
                lexer.goForward();
                break;
            }

            enterExpression();

            if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                lexer.goForward();
            } else if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {}else {
                Out::errorMessage(
                    lexer, "Expected ',', but found:\n\t" +
                               lexer.getCurrent()->str + "\tin " +
                               std::to_string(lexer.getCurrent()->line) + ":" +
                               std::to_string(lexer.getCurrent()->pos));
            }
        }
    } else if (lexer.getCurrent()->kind == Token::Kind::LBRACKET) {
        while (true) {
            lexer.goForward();

            if (lexer.getCurrent()->kind != Token::Kind::RBRACKET) {
                enterExpression();
            }

            if (lexer.getCurrent()->kind == Token::Kind::RBRACKET) {
                lexer.goForward();
                if (lexer.getCurrent()->kind == Token::Kind::LBRACKET) {
                    continue;
                } else {
                    break;
                }
            } else {
                Out::errorMessage(
                    lexer, "Expected ']', but found:\n\t" +
                               lexer.getCurrent()->str + "\tin " +
                               std::to_string(lexer.getCurrent()->line) + ":" +
                               std::to_string(lexer.getCurrent()->pos));
            }
        }
    }

    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        enterArrayInitializer();
    }
}

void SymbolListener::enterParenExpr() {
    if (lexer.getCurrent()->kind != Token::Kind::LPAREN) {
        Out::errorMessage(
            lexer, "Expected '(', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    enterExpression();
    if (lexer.getCurrent()->kind != Token::Kind::RPAREN) {
        Out::errorMessage(
            lexer, "Expected ')', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
}

string SymbolListener::enterAccessOrCall(bool arr) {
    string str = "";
    while (true) {
        str.append(enterAccessItem(arr));

        if (lexer.getCurrent()->kind == Token::Kind::DOT) {
            str.append(".");
            lexer.goForward();
        } else {
            break;
        }
    }
    return str;
}

string SymbolListener::enterAccessItem(bool arr) {
    string str = lexer.getCurrent()->str;
    lexer.goForward();

    //if (lexer.ifCurrTokenStartsWithLT()) {
    //    str += enterGeneric();
    //}
    if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        str += lexer.getCurrent()->str;
        lexer.goForward();
        while (true) {
            if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
                str += lexer.getCurrent()->str;
                lexer.goForward();
                break;
            }

            enterExpression();

            if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                str += lexer.getCurrent()->str;
                lexer.goForward();
            } else if (lexer.getCurrent()->kind != Token::Kind::RPAREN) {
                Out::errorMessage(
                    lexer, "Expected ',', but found:\n\t" +
                               lexer.getCurrent()->str + "\tin " +
                               std::to_string(lexer.getCurrent()->line) + ":" +
                               std::to_string(lexer.getCurrent()->pos));
            }
        }
    }
    if (arr) {
        if (lexer.getCurrent()->kind == Token::Kind::LBRACKET) {
            enterArrayAccess();
        }
    }
    return str;
}

void SymbolListener::enterArrayAccess() {
    lexer.goForward();
    enterExpression();
    if (lexer.getCurrent()->kind == Token::Kind::RBRACKET) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ']', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    if (lexer.getCurrent()->kind == Token::Kind::LBRACKET) {
        enterArrayAccess();
    }
}

string SymbolListener::enterGeneric() {
    string str = "<";
    lexer.removeFirstCharOfCurrShift();
    while (true) {
        if (lexer.ifCurrTokenStartsWithGT()) {
            str += ">";
            lexer.removeFirstCharOfCurrShift();
            break;
        }

        str += enterAccessOrCall(false);

        if (!lexer.ifCurrTokenStartsWithGT()) {
            if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                str += ",";
                lexer.goForward();
            } else {
                Out::errorMessage(
                    lexer, "Unexpected symbol:\n\t" + lexer.getCurrent()->str +
                               "\tin " +
                               std::to_string(lexer.getCurrent()->line) + ":" +
                               std::to_string(lexer.getCurrent()->pos));
            }
        }
    }
    return str;
}

void SymbolListener::enterLiteral() {
    if (lexer.getCurrent()->kind == Token::Kind::BOOL_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::CHAR_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::STRING_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_FLOAT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_FLOAT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_DOUBLE_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_DOUBLE_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_BYTE_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_BYTE_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_SHORT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_SHORT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_INT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_INT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_LONG_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_LONG_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::NULL_LITERAL) {
        lexer.goForward();
    }
}

void SymbolListener::enterArrayInitializer() {
    lexer.goForward();
    while (true) {
        if (lexer.getCurrent()->kind == Token::Kind::RBRACE) {
            lexer.goForward();
            break;
        }

        enterExpression();

        if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
            lexer.goForward();
        } else if (lexer.getCurrent()->kind != Token::Kind::RBRACE) {
            Out::errorMessage(
                lexer, "Unexpected symbol:\n\t" + lexer.getCurrent()->str +
                           "\tin " + std::to_string(lexer.getCurrent()->line) +
                           ":" + std::to_string(lexer.getCurrent()->pos));
        }
    }
}

vector<string> SymbolListener::enterExtending() {
    vector<string> superClasses{};
    if (lexer.getCurrent()->kind == Token::Kind::EXTENDS) {
        lexer.goForward();
        superClasses.push_back(enterType(true));
        if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
            Out::errorMessage(
                lexer, "Unexpected identifier ',' in " +
                           std::to_string(lexer.getCurrent()->line) + ":" +
                           std::to_string(lexer.getCurrent()->pos));
        } else if (lexer.getCurrent()->kind == Token::Kind::IMPLEMENTS) {
            lexer.goForward();
            while (true) {
                superClasses.push_back(enterType(true));
                if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                    lexer.goForward();
                } else {
                    break;
                }
            }
        }
    } else if (lexer.getCurrent()->kind == Token::Kind::IMPLEMENTS) {
        lexer.goForward();
        while (true) {
            superClasses.push_back(enterType(true));
            if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                lexer.goForward();
            } else {
                break;
            }
        }
    }
    return superClasses;
}

string SymbolListener::enterType(bool arr) {
    string str = enterAccessOrCall(false);

    int dims = 0;

    if (arr) {
        while (true) {
            if (lexer.getCurrent()->kind == Token::Kind::LBRACKET) {
                lexer.goForward();
                dims++;
                if (lexer.getCurrent()->kind == Token::Kind::RBRACKET) {
                    lexer.goForward();
                } else {
                    Out::errorMessage(
                        lexer, "Expected ']', but found:\n\t" +
                                   lexer.getCurrent()->str + "\tin " +
                                   std::to_string(lexer.getCurrent()->line) +
                                   ":" +
                                   std::to_string(lexer.getCurrent()->pos));
                }
            } else {
                break;
            }
        }
    }

    string dims_str = "";
    for (int i = 0; i < dims; ++i) {
        dims_str.append("[]");
    }

    return str + dims_str;
}

vector<ModifiersNode::ModifierKind> SymbolListener::enterModifiers() {
    vector<ModifiersNode::ModifierKind> mods =
        vector<ModifiersNode::ModifierKind>();
    while (true) {
        if (ParserUtils::isModifier(lexer.getCurrent())) {
            mods.push_back(modKinds.at(lexer.getCurrent()->str));
            lexer.goForward();
        } else {
            break;
        }
    }
    return mods;
}

map<string, ModifiersNode::ModifierKind> SymbolListener::modKinds =
    map<string, ModifiersNode::ModifierKind>(
        {{"public", ModifiersNode::ModifierKind::PUBLIC},
         {"private", ModifiersNode::ModifierKind::PRIVATE},
         {"protected", ModifiersNode::ModifierKind::PROTECTED},
         {"static", ModifiersNode::ModifierKind::STATIC},
         {"final", ModifiersNode::ModifierKind::FINAL},
         {"synchronized", ModifiersNode::ModifierKind::SYNCHRONIZED},
         {"abstract", ModifiersNode::ModifierKind::ABSTRACT},
         {"native", ModifiersNode::ModifierKind::NATIVE}});