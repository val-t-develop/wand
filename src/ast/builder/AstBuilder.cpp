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
 *  Builder of Abstract Syntax Tree (AST).
 */

#include "AstBuilder.hpp"
#include <ast/node/statement/expression/literal/BoolLiteralNode.hpp>
#include <ast/node/statement/expression/literal/CharLiteralNode.hpp>
#include <ast/node/statement/expression/literal/FloatLiteralNode.hpp>
#include <ast/node/statement/expression/literal/IntLiteralNode.hpp>
#include <ast/node/statement/expression/literal/NullNode.hpp>
#include <ast/node/statement/expression/literal/StringLiteralNode.hpp>
#include <utils/Out.hpp>
#include <utils/ParserUtils.hpp>

AstBuilder::AstBuilder(shared_ptr<SymbolTable> _symbolTable, Path _filePath,
                       Lexer _lexer)
    : symbolTable(_symbolTable), filePath(_filePath), lexer(_lexer) {}

shared_ptr<CompilationUnitNode> AstBuilder::walk() {
    shared_ptr<CompilationUnitNode> cu = make_shared<CompilationUnitNode>();
    ParserUtils::skipSemicolons(lexer);
    if (lexer.getCurrent()->kind == Token::Kind::PACKAGE) {
        shared_ptr<PackageDeclNode> p = enterPackage();
        p->parent = cu;
        cu->nodes.push_back(p);
    }
    while (true) {
        ParserUtils::skipSemicolons(lexer);
        if (lexer.getCurrent()->kind == Token::Kind::IMPORT) {
            shared_ptr<ImportDeclNode> i = enterImport();
            i->parent = cu;
            cu->nodes.push_back(i);
        } else {
            break;
        }
    }

    while (lexer.getCurrent()->kind != Token::Kind::END_OF_FILE) {
        ParserUtils::skipSemicolons(lexer);

        shared_ptr<ClassDeclNode> c = enterTypeDecl();
        c->parent = cu;
        cu->nodes.push_back(c);
    }
    return cu;
}

shared_ptr<PackageDeclNode> AstBuilder::enterPackage() {
    if (lexer.getCurrent()->kind == Token::Kind::PACKAGE) {
        lexer.goForward();
        shared_ptr<ParserUtils::QualifiedName> qualifiedName =
            make_shared<ParserUtils::QualifiedName>(lexer);
        return make_shared<PackageDeclNode>(qualifiedName->getTextList(),
                                            nullptr);
    }
    return nullptr;
}

shared_ptr<ImportDeclNode> AstBuilder::enterImport() {
    if (lexer.getCurrent()->kind == Token::Kind::IMPORT) {
        lexer.goForward();
        shared_ptr<ParserUtils::QualifiedName> qualifiedName =
            make_shared<ParserUtils::QualifiedName>(lexer);
        return make_shared<ImportDeclNode>(qualifiedName->getTextList(),
                                           nullptr);
    }
    return nullptr;
}

shared_ptr<ClassDeclNode> AstBuilder::enterTypeDecl() {
    shared_ptr<ModifiersNode> mods = enterModifiers();
    shared_ptr<ClassDeclNode> cl = nullptr;
    if (lexer.getCurrent()->kind == Token::Kind::CLASS) {
        cl = enterClassDecl(ClassDeclNode::ClassKind::CLASS);
    } else if (lexer.getCurrent()->kind == Token::Kind::INTERFACE) {
        cl = enterInterfaceDecl();
    } else if (lexer.getCurrent()->kind == Token::Kind::ENUM) {
        cl = enterEnumDecl();
    } else {
        Out::errorMessage(lexer,
                          "Expected type declaration kind specifier (class, "
                          "interface, enum or @interface), but found:\n\t" +
                              lexer.getCurrent()->str + "\tin " +
                              std::to_string(lexer.getCurrent()->line) + ":" +
                              std::to_string(lexer.getCurrent()->pos));
    }
    if (cl != nullptr) {
        cl->modifiers = mods;
    }
    return cl;
}

shared_ptr<ClassDeclNode>
AstBuilder::enterClassDecl(ClassDeclNode::ClassKind kind) {
    lexer.goForward();
    if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER) {
        string name = lexer.getCurrent()->str;
        lexer.goForward();

        symbolTable->enterScope(nullptr);

        shared_ptr<GenericNode> generic = nullptr;
        if (lexer.ifCurrTokenStartsWithLT()) {
            generic = enterGenericDecl();
        }

        shared_ptr<ClassRecord> record = symbolTable->lookupClass(name);

        vector<shared_ptr<TypeNode>> extended = vector<shared_ptr<TypeNode>>();
        vector<shared_ptr<TypeNode>> implemented =
            vector<shared_ptr<TypeNode>>();
        if (lexer.getCurrent()->kind == Token::Kind::EXTENDS) {
            lexer.goForward();
            extended = enterTypeList();
        }
        if (lexer.getCurrent()->kind == Token::Kind::IMPLEMENTS) {
            lexer.goForward();
            implemented = enterTypeList();
        }

        vector<shared_ptr<VarDeclNode>> fields =
            vector<shared_ptr<VarDeclNode>>();
        vector<shared_ptr<MethodDeclNode>> methods =
            vector<shared_ptr<MethodDeclNode>>();
        vector<shared_ptr<ClassDeclNode>> innerClasses =
            vector<shared_ptr<ClassDeclNode>>();
        vector<shared_ptr<ConstructorDeclNode>> constructors =
            vector<shared_ptr<ConstructorDeclNode>>();

        if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
            lexer.goForward();

            while (true) {
                ParserUtils::skipSemicolons(lexer);
                if (lexer.getCurrent()->kind == Token::Kind::RBRACE) {
                    lexer.goForward();
                    break;
                }

                shared_ptr<Node> n = enterClassMemberDecl();
                if (n->getKind() == Node::NodeKind::VAR_DECL_NODE) {
                    fields.push_back(static_pointer_cast<VarDeclNode>(n));
                } else if (n->getKind() == Node::NodeKind::METHOD_DECL_NODE) {
                    methods.push_back(static_pointer_cast<MethodDeclNode>(n));
                } else if (n->getKind() == Node::NodeKind::CLASS_DECL_NODE) {
                    innerClasses.push_back(
                        static_pointer_cast<ClassDeclNode>(n));
                } else if (n->getKind() ==
                           Node::NodeKind::CONSTRUCTOR_DECL_NODE) {
                    constructors.push_back(
                        static_pointer_cast<ConstructorDeclNode>(n));
                }
            }
            symbolTable->exitScope();
        } else {
            Out::errorMessage(
                lexer, "Expected '{', but found:\n\t" +
                           lexer.getCurrent()->str + "\tin " +
                           std::to_string(lexer.getCurrent()->line) + ":" +
                           std::to_string(lexer.getCurrent()->pos));
        }

        return make_shared<ClassDeclNode>(
            generic, nullptr, kind, record, extended, implemented, fields,
            methods, constructors, innerClasses, nullptr);
    } else {
        Out::errorMessage(lexer, "Expected identifier, but found:\n\t" +
                                     lexer.getCurrent()->str + "\tin " +
                                     std::to_string(lexer.getCurrent()->line) +
                                     ":" +
                                     std::to_string(lexer.getCurrent()->pos));
    }
    return nullptr;
}

shared_ptr<ClassDeclNode> AstBuilder::enterInterfaceDecl() {
    return enterClassDecl(ClassDeclNode::ClassKind::INTERFACE);
}

shared_ptr<ClassDeclNode> AstBuilder::enterEnumDecl() {
    return enterClassDecl(ClassDeclNode::ClassKind::ENUM);
}

shared_ptr<GenericNode> AstBuilder::enterGenericDecl() {
    vector<shared_ptr<ClassRecordNode>> list =
        vector<shared_ptr<ClassRecordNode>>();
    lexer.removeFirstCharOfCurrShift();

    while (true) {
        if (lexer.ifCurrTokenStartsWithGT()) {
            lexer.removeFirstCharOfCurrShift();
            break;
        }
        list.push_back(enterGenericTypeDecl());
        if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
            lexer.goForward();
        }
    }
    return make_shared<GenericNode>(list, nullptr);
}

shared_ptr<ClassRecordNode> AstBuilder::enterGenericTypeDecl() {
    if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER) {
        shared_ptr<ClassRecord> record =
            symbolTable->lookupClass(lexer.getCurrent()->str);
        lexer.goForward();
        return make_shared<ClassRecordNode>(
            record, vector<shared_ptr<AccessNode>>(), nullptr);
    } else {
        Out::errorMessage(lexer, "Expected identifier, but found:\n\t" +
                                     lexer.getCurrent()->str + "\tin " +
                                     std::to_string(lexer.getCurrent()->line) +
                                     ":" +
                                     std::to_string(lexer.getCurrent()->pos));
    }
    return nullptr;
}

shared_ptr<Node> AstBuilder::enterClassMemberDecl() {
    shared_ptr<ModifiersNode> mods = enterModifiers();
    if (lexer.getCurrent()->kind == Token::Kind::CLASS ||
        lexer.getCurrent()->kind == Token::Kind::INTERFACE ||
        lexer.getCurrent()->kind == Token::Kind::ENUM ||
        lexer.getCurrent()->kind == Token::Kind::AT) {
        shared_ptr<ClassDeclNode> n = enterTypeDecl();
        n->modifiers = mods;
        return n;
    } else if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        // TODO initializers
        return nullptr;
    } else {
        if (lexer.getNext()->kind == Token::Kind::LPAREN) {
            shared_ptr<ConstructorDeclNode> decl = enterConstructorDecl();
            decl->modifiers = mods;
            return decl;
        }
        shared_ptr<TypeNode> type = enterType(true);
        if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER) {
            if (lexer.getNext()->kind == Token::Kind::LPAREN) {
                shared_ptr<MethodRecord> record =
                    symbolTable->lookupMethod(lexer.getCurrent()->str);
                lexer.goForward();
                shared_ptr<MethodDeclNode> decl =
                    enterMethodDecl(type, (shared_ptr<MethodRecord>)record);
                decl->modifiers = mods;
                return decl;
            } else {
                shared_ptr<VarRecord> record =
                    symbolTable->lookupVar(lexer.getCurrent()->str);
                lexer.goForward();
                shared_ptr<VarDeclNode> decl =
                    enterFieldDecl(type, (shared_ptr<VarRecord>)record);
                decl->modifiers = mods;
                return decl;
            }
        } else {
            Out::errorMessage(
                lexer, "Expected identifier, but found:\n\t" +
                           lexer.getCurrent()->str + "\tin " +
                           std::to_string(lexer.getCurrent()->line) + ":" +
                           std::to_string(lexer.getCurrent()->pos));
        }
    }
    return nullptr;
}

shared_ptr<ConstructorDeclNode> AstBuilder::enterConstructorDecl() {
    lexer.goForward();
    symbolTable->enterScope(nullptr);
    vector<shared_ptr<VarDeclNode>> args = enterMethodArgs();
    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        shared_ptr<BlockNode> block = enterBlockStatement(false);
        symbolTable->exitScope();
        return make_shared<ConstructorDeclNode>(
            make_shared<ModifiersNode>(nullptr), nullptr, args, block, nullptr);
    } else {
        Out::errorMessage(
            lexer, "Expected '{', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    symbolTable->exitScope();
    return nullptr;
}

shared_ptr<MethodDeclNode>
AstBuilder::enterMethodDecl(shared_ptr<TypeNode> type,
                            shared_ptr<MethodRecord> record) {
    symbolTable->enterScope(nullptr);
    vector<shared_ptr<VarDeclNode>> args = enterMethodArgs();

    bool found = true;

    if (record->argsCount == args.size()) {
        for (int i = 0; i < args.size(); ++i) {
            if (args[i]->record != record->vars[i]) {
                found = false;
                break;
            }
        }
    } else {
        found = false;
    }

    if (!found) {
        for (auto rec : record->similar) {
            found = true;
            if (rec->argsCount == args.size()) {
                for (int i = 0; i < args.size(); ++i) {
                    if (args[i]->record != rec->vars[i]) {
                        found = false;
                        break;
                    }
                }
            } else {
                found = false;
            }
            if (found) {
                record = rec;
                break;
            }
        }
    }
    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        shared_ptr<BlockNode> block = enterBlockStatement(false);
        symbolTable->exitScope();
        return make_shared<MethodDeclNode>(make_shared<ModifiersNode>(nullptr),
                                           type, record, args, block, nullptr);
    } else if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        symbolTable->exitScope();
        return make_shared<MethodDeclNode>(make_shared<ModifiersNode>(nullptr),
                                           type, record, args, nullptr,
                                           nullptr);
    } else {
        Out::errorMessage(
            lexer, "Expected '{', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    symbolTable->exitScope();
    return nullptr;
}

shared_ptr<VarDeclNode>
AstBuilder::enterFieldDecl(shared_ptr<TypeNode> type,
                           shared_ptr<VarRecord> record) {
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
    type->dims = type->dims + dims;
    record->typeRec = type->type->record;

    shared_ptr<ExpressionNode> init = nullptr;
    if (lexer.getCurrent()->kind == Token::Kind::ASSIGN) {
        lexer.goForward();
        init = enterExpression();
    }
    return make_shared<VarDeclNode>(nullptr, type, record, init, nullptr);
}

vector<shared_ptr<VarDeclNode>> AstBuilder::enterMethodArgs() {
    vector<shared_ptr<VarDeclNode>> args = vector<shared_ptr<VarDeclNode>>();
    lexer.goForward();
    while (true) {
        if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
            lexer.goForward();
            break;
        }
        shared_ptr<ModifiersNode> mods = enterModifiers();
        shared_ptr<TypeNode> type = enterType(true);
        if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER) {
            shared_ptr<VarRecord> record =
                symbolTable->lookupVar(lexer.getCurrent()->str); // TODO
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

            type->dims = type->dims + dims;
            record->typeRec = type->type->record;

            args.push_back(
                make_shared<VarDeclNode>(mods, type, record, nullptr, nullptr));
        } else {
            Out::errorMessage(
                lexer, "Expected identifier, but found:\n\t" +
                           lexer.getCurrent()->str + "\tin " +
                           std::to_string(lexer.getCurrent()->line) + ":" +
                           std::to_string(lexer.getCurrent()->pos));
        }
        if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
            lexer.goForward();
        } else {
            if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
                lexer.goForward();
                break;
            } else {
                Out::errorMessage(lexer, "Expected ')'");
            }
        }
    }
    return args;
}

shared_ptr<StatementNode> AstBuilder::enterStatement() {
    shared_ptr<ModifiersNode> mods = enterModifiers();
    if (lexer.getCurrent()->kind == Token::Kind::CLASS) {
        // TODO
    } else if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER ||
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
                return enterLocalVarDecl(mods);
            } else if (lexer.getWithOffset(i)->kind == Token::Kind::LBRACKET ||
                       lexer.getWithOffset(i)->kind == Token::Kind::RBRACKET) {
                i++;
            } else {
                return enterNotVarStartement();
            }
        }
    } else {
        return enterNotVarStartement();
    }
    return nullptr;
}

shared_ptr<StatementNode> AstBuilder::enterNotVarStartement() {
    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        return enterBlockStatement(true);
    } else if (lexer.getCurrent()->kind == Token::Kind::ASSERT) {
        return enterAssertStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::BREAK) {
        return enterBreakStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::CONTINUE) {
        return enterContinueStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::RETURN) {
        return enterReturnStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::IF) {
        return enterIfStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::WHILE) {
        return enterWhileStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::FOR) {
        return enterForStatement();
    } else if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
        return nullptr;
    } else {
        return enterExpression();
    }
}

shared_ptr<VarsDeclNode>
AstBuilder::enterLocalVarDecl(shared_ptr<ModifiersNode> mods) {
    shared_ptr<TypeNode> type = enterType(true);
    vector<shared_ptr<VarDeclNode>> decls = vector<shared_ptr<VarDeclNode>>();
    while (true) {
        if (lexer.getCurrent()->kind == Token::Kind::IDENTIFIER) {
            shared_ptr<VarRecord> record =
                symbolTable->lookupVar(lexer.getCurrent()->str);
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

            type->dims = type->dims + dims;
            record->typeRec = type->type->record;

            shared_ptr<ExpressionNode> init = nullptr;
            if (lexer.getCurrent()->kind == Token::Kind::ASSIGN) {
                lexer.goForward();
                init = enterExpression();
            }
            decls.push_back(
                make_shared<VarDeclNode>(mods, type, record, init, nullptr));

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

    return make_shared<VarsDeclNode>(decls, nullptr);
}

shared_ptr<BlockNode> AstBuilder::enterBlockStatement(bool enterScope) {
    if (enterScope) {
        symbolTable->enterScope(nullptr);
    }

    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        lexer.goForward();
        vector<shared_ptr<Node>> nodes = vector<shared_ptr<Node>>();
        while (true) {
            ParserUtils::skipSemicolons(lexer);
            if (lexer.getCurrent()->kind == Token::Kind::RBRACE) {
                if (enterScope) {
                    symbolTable->exitScope();
                }
                lexer.goForward();
                break;
            }
            nodes.push_back(enterStatement());
        }
        return make_shared<BlockNode>(nodes, nullptr);
    } else {
        Out::errorMessage(
            lexer, "Expected '{', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    return nullptr;
}

shared_ptr<AssertNode> AstBuilder::enterAssertStatement() {
    lexer.goForward();
    shared_ptr<ExpressionNode> condition = enterExpression();
    shared_ptr<ExpressionNode> expression = nullptr;
    if (lexer.getCurrent()->kind == Token::Kind::COLON) {
        lexer.goForward();
        expression = enterExpression();
    }

    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    return make_shared<AssertNode>(condition, expression, nullptr);
}

shared_ptr<BreakNode> AstBuilder::enterBreakStatement() {
    lexer.goForward();
    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    return make_shared<BreakNode>(nullptr);
}

shared_ptr<ContinueNode> AstBuilder::enterContinueStatement() {
    lexer.goForward();
    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    return make_shared<ContinueNode>(nullptr);
}

shared_ptr<ReturnNode> AstBuilder::enterReturnStatement() {
    lexer.goForward();
    shared_ptr<ExpressionNode> expression = nullptr;

    if (lexer.getCurrent()->kind != Token::Kind::SEMICOLON) {
        expression = enterExpression();
    }

    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    return make_shared<ReturnNode>(expression, nullptr);
}

shared_ptr<IfElseNode> AstBuilder::enterIfStatement() {
    lexer.goForward();

    if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected '(', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    shared_ptr<ExpressionNode> condition = enterExpression();

    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ')', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    shared_ptr<StatementNode> thenStatement = enterStatement();
    shared_ptr<StatementNode> elseStatement = nullptr;

    if (lexer.getCurrent()->kind == Token::Kind::ELSE) {
        lexer.goForward();
        elseStatement = enterStatement();
    }

    return make_shared<IfElseNode>(condition, thenStatement, elseStatement,
                                   nullptr);
}

shared_ptr<WhileNode> AstBuilder::enterWhileStatement() {
    lexer.goForward();

    if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected '(', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    shared_ptr<ExpressionNode> condition = enterExpression();

    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ')', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    shared_ptr<StatementNode> statement = enterStatement();

    return make_shared<WhileNode>(condition, statement, nullptr);
}

shared_ptr<ForNode> AstBuilder::enterForStatement() {
    lexer.goForward();

    if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected '(', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    shared_ptr<StatementNode> init = enterStatement();
    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    shared_ptr<ExpressionNode> condition = enterExpression();
    if (lexer.getCurrent()->kind == Token::Kind::SEMICOLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ';', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    shared_ptr<StatementNode> update = nullptr;
    if (lexer.getCurrent()->kind != Token::Kind::RPAREN) {
        update = enterStatement();
    }

    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ')', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    shared_ptr<StatementNode> statement = enterStatement();

    return make_shared<ForNode>(init, condition, update, statement, nullptr);
}

shared_ptr<ForEachNode> AstBuilder::enterForEachStatement() {
    lexer.goForward();

    if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected '(', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    shared_ptr<ModifiersNode> mods = enterModifiers();
    shared_ptr<VarDeclNode> var = enterLocalVarDecl(mods)->decls.at(0);

    if (lexer.getCurrent()->kind == Token::Kind::COLON) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ':', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    shared_ptr<ExpressionNode> expression = enterExpression();

    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
        lexer.goForward();
    } else {
        Out::errorMessage(
            lexer, "Expected ')', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }

    shared_ptr<StatementNode> statement = enterStatement();

    return make_shared<ForEachNode>(var, expression, statement, nullptr);
}

shared_ptr<ExpressionNode> AstBuilder::enterExpression() {
    shared_ptr<ExpressionNode> LHS = enterUnOpPrimary();
    if (LHS == nullptr) {
        return nullptr;
    }
    return enterBinOpRHS(0, LHS);
}

shared_ptr<ExpressionNode>
AstBuilder::enterBinOpRHS(int exprPrec, shared_ptr<ExpressionNode> LHS) {
    while (true) {
        int tokPrec = ParserUtils::getBinOpPrecedence(lexer);

        if (tokPrec < exprPrec)
            return LHS;

        string binOp = lexer.getCurrent()->str;
        lexer.goForward();

        shared_ptr<ExpressionNode> RHS = enterUnOpPrimary();
        if (RHS == nullptr)
            return nullptr;

        int nextPrec = ParserUtils::getBinOpPrecedence(lexer);
        if (tokPrec < nextPrec) {
            RHS = enterBinOpRHS(tokPrec + 1, RHS);
            if (RHS == nullptr)
                return nullptr;
        }

        LHS = make_shared<BinaryOperatorNode>(
            LHS, RHS, BinaryOperatorNode::operatorKinds.at(binOp), nullptr);
    }
}

shared_ptr<ExpressionNode> AstBuilder::enterUnOpPrimary() {
    int preInc = 0;
    int preDec = 0;
    int postInc = 0;
    int postDec = 0;
    int bang = 0;
    int tilde = 0;
    int plus = 0;
    int minus = 0;

    while (true) {
        if (lexer.getCurrent()->kind == Token::Kind::INC) {
            preInc++;
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::DEC) {
            preDec++;
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::BANG) {
            bang++;
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::TILDE) {
            tilde++;
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::ADD) {
            plus++;
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::SUB) {
            minus++;
            lexer.goForward();
        } else {
            break;
        }
    }

    shared_ptr<ExpressionNode> primary = enterPrimary();

    while (true) {
        if (lexer.getCurrent()->kind == Token::Kind::INC) {
            postInc++;
            lexer.goForward();
        } else if (lexer.getCurrent()->kind == Token::Kind::DEC) {
            postDec++;
            lexer.goForward();
        } else {
            break;
        }
    }

    for (int i = preInc; i > 0; i--) {
        primary = make_shared<UnaryOperatorNode>(true, "++", primary, nullptr);
    }
    for (int i = preDec; i > 0; i--) {
        primary = make_shared<UnaryOperatorNode>(true, "--", primary, nullptr);
    }

    for (int i = plus; i > 0; i--) {
        primary = make_shared<UnaryOperatorNode>(true, "+", primary, nullptr);
    }
    for (int i = minus; i > 0; i--) {
        primary = make_shared<UnaryOperatorNode>(true, "-", primary, nullptr);
    }

    for (int i = bang; i > 0; i--) {
        primary = make_shared<UnaryOperatorNode>(true, "!", primary, nullptr);
    }
    for (int i = tilde; i > 0; i--) {
        primary = make_shared<UnaryOperatorNode>(true, "~", primary, nullptr);
    }

    for (int i = postInc; i > 0; i--) {
        primary = make_shared<UnaryOperatorNode>(false, "++", primary, nullptr);
    }
    for (int i = postDec; i > 0; i--) {
        primary = make_shared<UnaryOperatorNode>(false, "--", primary, nullptr);
    }
    return primary;
}

shared_ptr<ExpressionNode> AstBuilder::enterPrimary() {
    if (lexer.getCurrent()->kind == Token::Kind::BOOL_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::CHAR_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::STRING_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_FLOAT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_FLOAT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_DOUBLE_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_DOUBLE_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_INT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_INT_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::DEC_LONG_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::HEX_LONG_LITERAL ||
        lexer.getCurrent()->kind == Token::Kind::NULL_LITERAL) {
        return enterLiteral();
    } else if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        return enterParenExpression();
    } else if (lexer.getCurrent()->kind == Token::Kind::NEW) {
        return enterNew();
    } else if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        return enterArrayInitializer();
    } else {
        return enterAccessOrCall(true);
    }
}

shared_ptr<ExpressionNode> AstBuilder::enterNew() {
    lexer.goForward();

    bool isStatic = false;
    if (lexer.getCurrent()->kind == Token::Kind::STATIC) {
        isStatic = true;
        lexer.goForward();
    }

    shared_ptr<TypeNode> type = enterType(false);

    vector<shared_ptr<ExpressionNode>> args =
        vector<shared_ptr<ExpressionNode>>();

    bool arr = false;

    if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
        lexer.goForward();

        while (true) {
            if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
                lexer.goForward();
                break;
            }

            args.push_back(enterExpression());

            if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                lexer.goForward();
            } else if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {} else {
                Out::errorMessage(
                    lexer, "Expected ',', but found:\n\t" +
                               lexer.getCurrent()->str + "\tin " +
                               std::to_string(lexer.getCurrent()->line) + ":" +
                               std::to_string(lexer.getCurrent()->pos));
            }
        }
    } else if (lexer.getCurrent()->kind == Token::Kind::LBRACKET) {
        arr = true;

        while (true) {
            lexer.goForward();

            if (lexer.getCurrent()->kind != Token::Kind::RBRACKET) {
                args.push_back(enterExpression());
            } else {
                args.push_back(nullptr);
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

    shared_ptr<ArrayInitializerNode> arrayInitializer = nullptr;

    if (lexer.getCurrent()->kind == Token::Kind::LBRACE) {
        if (!arr) {
            Out::errorMessage(
                lexer, "Unexpected symbol:\n\t" + lexer.getCurrent()->str +
                           "\tin " + std::to_string(lexer.getCurrent()->line) +
                           ":" + std::to_string(lexer.getCurrent()->pos));
        }
        arrayInitializer = enterArrayInitializer();
    }

    if (arr) {
        return make_shared<ArrayCreationNode>(type, args, arrayInitializer,
                                              isStatic, nullptr);
    } else {
        return make_shared<NewNode>(type, args, isStatic, nullptr);
    }
}

shared_ptr<ExpressionNode> AstBuilder::enterParenExpression() {
    if (lexer.getCurrent()->kind != Token::Kind::LPAREN) {
        Out::errorMessage(
            lexer, "Expected '(', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    shared_ptr<ExpressionNode> expr = enterExpression();
    if (lexer.getCurrent()->kind != Token::Kind::RPAREN) {
        Out::errorMessage(
            lexer, "Expected ')', but found:\n\t" + lexer.getCurrent()->str +
                       "\tin " + std::to_string(lexer.getCurrent()->line) +
                       ":" + std::to_string(lexer.getCurrent()->pos));
    }
    return expr;
}

shared_ptr<AccessNode> AstBuilder::enterAccessOrCall(bool arr) {
    shared_ptr<AccessNode> access = make_shared<AccessNode>(nullptr);
    while (true) {
        enterAccessWithoutArray(access);
        if (arr) {
            if (lexer.getCurrent()->kind == Token::Kind::LBRACKET) {
                shared_ptr<ArrayAccessNode> arrayAccessNode =
                    make_shared<ArrayAccessNode>(
                        access, vector<shared_ptr<ExpressionNode>>{}, nullptr);
                while (true) {
                    lexer.goForward();
                    arrayAccessNode->indexes.push_back(enterExpression());
                    lexer.goForward();
                    if (lexer.getCurrent()->kind != Token::Kind::RBRACKET) {
                        break;
                    }
                }
                access = make_shared<AccessNode>(nullptr);
                access->access.push_back(arrayAccessNode);
            } else {
                break;
            }
        } else {
            break;
        }
    }
    return access;
}

void AstBuilder::enterAccessWithoutArray(shared_ptr<AccessNode> access) {
    string name = lexer.getCurrent()->str;
    shared_ptr<VarRecord> varRecord = symbolTable->lookupVar(name);
    shared_ptr<ClassRecord> classRecord = symbolTable->lookupClass(name);
    shared_ptr<MethodRecord> methodRecord = symbolTable->lookupMethod(name);
    shared_ptr<Record> record = symbolTable->lookupRecord(name);
    lexer.goForward();

    Record::RecordKind kind = Record::RecordKind::UNUSED;
    if (varRecord != nullptr) {
        kind = varRecord->kind;
        access->access.push_back(
            make_shared<VarRecordNode>(varRecord, nullptr));
    } else if (classRecord != nullptr) {
        access->access.push_back(make_shared<ClassRecordNode>(
            classRecord, vector<shared_ptr<AccessNode>>{}, nullptr));
        kind = classRecord->kind;
    } else if (methodRecord != nullptr) {
        kind = methodRecord->kind;

        // TODO method reference
        shared_ptr<MethodCallNode> call = make_shared<MethodCallNode>(
            methodRecord, vector<shared_ptr<ExpressionNode>>(), nullptr);
        if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
            lexer.goForward();
        } else {
            Out::errorMessage(
                lexer, "Expected '(', but found:\n\t" +
                           lexer.getCurrent()->str + "\tin " +
                           std::to_string(lexer.getCurrent()->line) + ":" +
                           std::to_string(lexer.getCurrent()->pos));
        }

        vector<shared_ptr<ExpressionNode>> args =
            vector<shared_ptr<ExpressionNode>>();
        while (true) {
            if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
                lexer.goForward();
                break;
            }

            args.push_back(enterExpression());

            if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                lexer.goForward();
            } else if (lexer.getCurrent()->kind != Token::Kind::RPAREN) {
                Out::errorMessage(
                    lexer, "Expected ',', but found:\n\t" +
                               lexer.getCurrent()->str + "\tin " +
                               std::to_string(lexer.getCurrent()->line) + ":" +
                               std::to_string(lexer.getCurrent()->pos));
            }
        }

        call->args = args;

        access->access.push_back(call);
    }

    while (lexer.getCurrent()->str == ".") {
        lexer.goForward();
        string new_name = lexer.getCurrent()->str;
        lexer.goForward();

        if (kind == Record::RecordKind::LOCAL_VAR_RECORD) {
            shared_ptr<VarRecordNode> node = static_pointer_cast<VarRecordNode>(
                access->access[access->access.size() - 1]);
            shared_ptr<ClassRecord> type = node->record->typeRec;
            shared_ptr<Record> new_record = type->get(new_name);
            Record::RecordKind new_kind = new_record->kind;

            if (new_kind == Record::RecordKind::LOCAL_VAR_RECORD) {
                Out::errorMessage(
                    "Unexpected access: local variable.local variable");
            } else if (new_kind == Record::RecordKind::FIELD_RECORD) {
                shared_ptr<VarRecord> field =
                    static_pointer_cast<VarRecord>(new_record);
                shared_ptr<VarRecordNode> fieldNode =
                    make_shared<VarRecordNode>(field, nullptr);
                access->access.push_back(fieldNode);
            } else if (new_kind == Record::RecordKind::METHOD_RECORD) {
                shared_ptr<MethodRecord> new_methodRecord =
                    static_pointer_cast<MethodRecord>(new_record);
                // TODO method reference
                shared_ptr<MethodCallNode> call = make_shared<MethodCallNode>(
                    new_methodRecord, vector<shared_ptr<ExpressionNode>>(),
                    nullptr);
                if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
                    lexer.goForward();
                } else {
                    Out::errorMessage(
                        lexer, "Expected '(', but found:\n\t" +
                                   lexer.getCurrent()->str + "\tin " +
                                   std::to_string(lexer.getCurrent()->line) +
                                   ":" +
                                   std::to_string(lexer.getCurrent()->pos));
                }

                vector<shared_ptr<ExpressionNode>> args =
                    vector<shared_ptr<ExpressionNode>>();
                while (true) {
                    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
                        lexer.goForward();
                        break;
                    }

                    args.push_back(enterExpression());

                    if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                        lexer.goForward();
                    } else if (lexer.getCurrent()->kind !=
                               Token::Kind::RPAREN) {
                        Out::errorMessage(
                            lexer,
                            "Expected ',', but found:\n\t" +
                                lexer.getCurrent()->str + "\tin " +
                                std::to_string(lexer.getCurrent()->line) + ":" +
                                std::to_string(lexer.getCurrent()->pos));
                    }
                }

                call->args = args;

                access->access.push_back(call);
            } else if (new_kind == Record::RecordKind::CLASS_RECORD) {
                Out::errorMessage("Unexpected access: local variable.class");
            } else if (new_kind == Record::RecordKind::UNUSED) {
                Out::errorMessage("Error: using of unused record!");
            }
            kind = new_kind;
        } else if (kind == Record::RecordKind::FIELD_RECORD) {
            shared_ptr<VarRecordNode> node = static_pointer_cast<VarRecordNode>(
                access->access[access->access.size() - 1]);
            shared_ptr<ClassRecord> type = node->record->typeRec;
            shared_ptr<Record> new_record = type->get(new_name);
            Record::RecordKind new_kind = new_record->kind;

            if (new_kind == Record::RecordKind::LOCAL_VAR_RECORD) {
                Out::errorMessage("Unexpected access: field.local variable");
            } else if (new_kind == Record::RecordKind::FIELD_RECORD) {
                shared_ptr<VarRecord> field =
                    static_pointer_cast<VarRecord>(new_record);
                shared_ptr<VarRecordNode> fieldNode =
                    make_shared<VarRecordNode>(field, nullptr);
                access->access.push_back(fieldNode);
            } else if (new_kind == Record::RecordKind::METHOD_RECORD) {
                shared_ptr<MethodRecord> new_methodRecord =
                    static_pointer_cast<MethodRecord>(new_record);
                // TODO method reference
                shared_ptr<MethodCallNode> call = make_shared<MethodCallNode>(
                    new_methodRecord, vector<shared_ptr<ExpressionNode>>(),
                    nullptr);
                if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
                    lexer.goForward();
                } else {
                    Out::errorMessage(
                        lexer, "Expected '(', but found:\n\t" +
                                   lexer.getCurrent()->str + "\tin " +
                                   std::to_string(lexer.getCurrent()->line) +
                                   ":" +
                                   std::to_string(lexer.getCurrent()->pos));
                }

                vector<shared_ptr<ExpressionNode>> args =
                    vector<shared_ptr<ExpressionNode>>();
                while (true) {
                    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
                        lexer.goForward();
                        break;
                    }

                    args.push_back(enterExpression());

                    if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                        lexer.goForward();
                    } else if (lexer.getCurrent()->kind !=
                               Token::Kind::RPAREN) {
                        Out::errorMessage(
                            lexer,
                            "Expected ',', but found:\n\t" +
                                lexer.getCurrent()->str + "\tin " +
                                std::to_string(lexer.getCurrent()->line) + ":" +
                                std::to_string(lexer.getCurrent()->pos));
                    }
                }

                call->args = args;

                access->access.push_back(call);
            } else if (new_kind == Record::RecordKind::CLASS_RECORD) {
                Out::errorMessage("Unexpected access: local variable.class");
            } else if (new_kind == Record::RecordKind::UNUSED) {
                Out::errorMessage("Error: using of unused record!");
            }
            kind = new_kind;
        } else if (kind == Record::RecordKind::METHOD_RECORD) {
            shared_ptr<MethodCallNode> node =
                static_pointer_cast<MethodCallNode>(
                    access->access[access->access.size() - 1]);
            shared_ptr<ClassRecord> type = node->record->retTypeRec;
            shared_ptr<Record> new_record = type->get(new_name);
            Record::RecordKind new_kind = new_record->kind;

            if (new_kind == Record::RecordKind::LOCAL_VAR_RECORD) {
                Out::errorMessage("Unexpected access: method.local variable");
            } else if (new_kind == Record::RecordKind::FIELD_RECORD) {
                shared_ptr<VarRecord> field =
                    static_pointer_cast<VarRecord>(new_record);
                shared_ptr<VarRecordNode> fieldNode =
                    make_shared<VarRecordNode>(field, nullptr);
                access->access.push_back(fieldNode);
            } else if (new_kind == Record::RecordKind::METHOD_RECORD) {
                shared_ptr<MethodRecord> new_methodRecord =
                    static_pointer_cast<MethodRecord>(new_record);
                // TODO method reference
                shared_ptr<MethodCallNode> call = make_shared<MethodCallNode>(
                    new_methodRecord, vector<shared_ptr<ExpressionNode>>(),
                    nullptr);
                if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
                    lexer.goForward();
                } else {
                    Out::errorMessage(
                        lexer, "Expected '(', but found:\n\t" +
                                   lexer.getCurrent()->str + "\tin " +
                                   std::to_string(lexer.getCurrent()->line) +
                                   ":" +
                                   std::to_string(lexer.getCurrent()->pos));
                }

                vector<shared_ptr<ExpressionNode>> args =
                    vector<shared_ptr<ExpressionNode>>();
                while (true) {
                    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
                        lexer.goForward();
                        break;
                    }

                    args.push_back(enterExpression());

                    if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                        lexer.goForward();
                    } else if (lexer.getCurrent()->kind !=
                               Token::Kind::RPAREN) {
                        Out::errorMessage(
                            lexer,
                            "Expected ',', but found:\n\t" +
                                lexer.getCurrent()->str + "\tin " +
                                std::to_string(lexer.getCurrent()->line) + ":" +
                                std::to_string(lexer.getCurrent()->pos));
                    }
                }

                call->args = args;

                access->access.push_back(call);
            } else if (new_kind == Record::RecordKind::CLASS_RECORD) {
                Out::errorMessage("Unexpected access: method.class");
            } else if (new_kind == Record::RecordKind::UNUSED) {
                Out::errorMessage("Error: using of unused record!");
            }
            kind = new_kind;
        } else if (kind == Record::RecordKind::CLASS_RECORD) {
            shared_ptr<ClassRecordNode> node =
                static_pointer_cast<ClassRecordNode>(
                    access->access[access->access.size() - 1]);
            shared_ptr<Record> new_record = node->record->get(new_name);
            Record::RecordKind new_kind = new_record->kind;

            if (new_kind == Record::RecordKind::LOCAL_VAR_RECORD) {
                Out::errorMessage("Unexpected access: class.local variable");
            } else if (new_kind == Record::RecordKind::FIELD_RECORD) {
                shared_ptr<VarRecord> field =
                    static_pointer_cast<VarRecord>(new_record);
                shared_ptr<VarRecordNode> fieldNode =
                    make_shared<VarRecordNode>(field, nullptr);
                access->access[access->access.size() - 1] = fieldNode;
            } else if (new_kind == Record::RecordKind::METHOD_RECORD) {
                shared_ptr<MethodRecord> new_methodRecord =
                    static_pointer_cast<MethodRecord>(new_record);
                // TODO method reference
                shared_ptr<MethodCallNode> call = make_shared<MethodCallNode>(
                    new_methodRecord, vector<shared_ptr<ExpressionNode>>(),
                    nullptr);
                if (lexer.getCurrent()->kind == Token::Kind::LPAREN) {
                    lexer.goForward();
                } else {
                    Out::errorMessage(
                        lexer, "Expected '(', but found:\n\t" +
                                   lexer.getCurrent()->str + "\tin " +
                                   std::to_string(lexer.getCurrent()->line) +
                                   ":" +
                                   std::to_string(lexer.getCurrent()->pos));
                }

                vector<shared_ptr<ExpressionNode>> args =
                    vector<shared_ptr<ExpressionNode>>();
                while (true) {
                    if (lexer.getCurrent()->kind == Token::Kind::RPAREN) {
                        lexer.goForward();
                        break;
                    }

                    args.push_back(enterExpression());

                    if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
                        lexer.goForward();
                    } else if (lexer.getCurrent()->kind !=
                               Token::Kind::RPAREN) {
                        Out::errorMessage(
                            lexer,
                            "Expected ',', but found:\n\t" +
                                lexer.getCurrent()->str + "\tin " +
                                std::to_string(lexer.getCurrent()->line) + ":" +
                                std::to_string(lexer.getCurrent()->pos));
                    }
                }

                call->args = args;

                access->access[access->access.size() - 1] = call;
            } else if (new_kind == Record::RecordKind::CLASS_RECORD) {
                shared_ptr<ClassRecord> classRec =
                    static_pointer_cast<ClassRecord>(new_record);
                shared_ptr<ClassRecordNode> classNode =
                    make_shared<ClassRecordNode>(
                        classRec, vector<shared_ptr<AccessNode>>(), nullptr);
                access->access[access->access.size() - 1] = classNode;
            } else if (new_kind == Record::RecordKind::UNUSED) {
                Out::errorMessage("Error: using of unused record!");
            }
            kind = new_kind;
        } else if (kind == Record::RecordKind::UNUSED) {
            Out::errorMessage("Error: using of unused record!");
        }
    }
}

void AstBuilder::enterGeneric(shared_ptr<ClassRecordNode> classRecordNode) {
    lexer.removeFirstCharOfCurrShift();
    vector<shared_ptr<AccessNode>> typeArgs = vector<shared_ptr<AccessNode>>();
    while (true) {
        if (lexer.ifCurrTokenStartsWithGT()) {
            lexer.removeFirstCharOfCurrShift();
            break;
        }

        typeArgs.push_back(enterAccessOrCall());

        if (!lexer.ifCurrTokenStartsWithGT()) {
            if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
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
    classRecordNode->typeArgs = typeArgs;
}

shared_ptr<ExpressionNode> AstBuilder::enterLiteral() {
    if (lexer.getCurrent()->kind == Token::Kind::BOOL_LITERAL) {
        if (lexer.getCurrent()->str == "true") {
            lexer.goForward();
            return make_shared<BoolLiteralNode>(true, symbolTable->lookupClass("bool"), nullptr);
        } else {
            lexer.goForward();
            return make_shared<BoolLiteralNode>(false, symbolTable->lookupClass("bool"), nullptr);
        }
    } else if (lexer.getCurrent()->kind == Token::Kind::CHAR_LITERAL) {
        string str = lexer.getCurrent()->str;
        lexer.goForward();
        return make_shared<CharLiteralNode>(str, symbolTable->lookupClass("char"), nullptr);
    } else if (lexer.getCurrent()->kind == Token::Kind::STRING_LITERAL) {
        string str = lexer.getCurrent()->str;
        lexer.goForward();
        return make_shared<StringLiteralNode>(str, symbolTable->lookupClass("String"), nullptr);
    } else if (lexer.getCurrent()->kind == Token::Kind::DEC_FLOAT_LITERAL ||
               lexer.getCurrent()->kind == Token::Kind::HEX_FLOAT_LITERAL ||
               lexer.getCurrent()->kind == Token::Kind::DEC_DOUBLE_LITERAL ||
               lexer.getCurrent()->kind == Token::Kind::HEX_DOUBLE_LITERAL) {
        long double val = ParserUtils::parseDouble(lexer.getCurrent()->str);
        lexer.goForward();
        return make_shared<FloatLiteralNode>(val, true, symbolTable->lookupClass("float"), symbolTable->lookupClass("double"), nullptr);
    } else if (lexer.getCurrent()->kind == Token::Kind::DEC_INT_LITERAL ||
               lexer.getCurrent()->kind == Token::Kind::HEX_INT_LITERAL) {
        int64_t val = ParserUtils::parseLong(lexer.getCurrent()->str);
        lexer.goForward();
        return make_shared<IntLiteralNode>(val, false, symbolTable->lookupClass("int"), nullptr);
    } else if (lexer.getCurrent()->kind == Token::Kind::DEC_LONG_LITERAL ||
               lexer.getCurrent()->kind == Token::Kind::HEX_LONG_LITERAL) {
        int64_t val = ParserUtils::parseLong(lexer.getCurrent()->str);
        lexer.goForward();
        return make_shared<IntLiteralNode>(val, true, symbolTable->lookupClass("long"), nullptr);
    } else if (lexer.getCurrent()->kind == Token::Kind::NULL_LITERAL) {
        lexer.goForward();
        return make_shared<NullLiteralNode>(nullptr);
    }
    return nullptr;
}

shared_ptr<ArrayInitializerNode> AstBuilder::enterArrayInitializer() {
    vector<shared_ptr<ExpressionNode>> init =
        vector<shared_ptr<ExpressionNode>>();

    lexer.goForward();
    while (true) {
        if (lexer.getCurrent()->kind == Token::Kind::RBRACE) {
            lexer.goForward();
            break;
        }

        init.push_back(enterExpression());

        if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
            lexer.goForward();
        } else if (lexer.getCurrent()->kind != Token::Kind::RBRACE) {
            Out::errorMessage(
                lexer, "Unexpected symbol:\n\t" + lexer.getCurrent()->str +
                           "\tin " + std::to_string(lexer.getCurrent()->line) +
                           ":" + std::to_string(lexer.getCurrent()->pos));
        }
    }
    return make_shared<ArrayInitializerNode>(init, nullptr);
}

shared_ptr<TypeNode> AstBuilder::enterType(bool arr) {
    shared_ptr<AccessNode> type = enterAccessOrCall();

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
    }
    return make_shared<TypeNode>(
        static_pointer_cast<ClassRecordNode>(type->access[0]), dims,
        nullptr);
}

vector<shared_ptr<TypeNode>> AstBuilder::enterTypeList() {
    vector<shared_ptr<TypeNode>> list = vector<shared_ptr<TypeNode>>();
    while (true) {
        list.push_back(enterType(true));
        if (lexer.getCurrent()->kind == Token::Kind::COMMA) {
            lexer.goForward();
        } else {
            break;
        }
    }
    return list;
}

shared_ptr<ModifiersNode> AstBuilder::enterModifiers() {
    shared_ptr<ModifiersNode> mods = make_shared<ModifiersNode>(nullptr);
    while (true) {
        if (ParserUtils::isModifier(lexer.getCurrent())) {
            mods->modifiers.push_back(modKinds.at(lexer.getCurrent()->str));
            lexer.goForward();
        } else {
            break;
        }
    }
    return mods;
}

map<string, ModifiersNode::ModifierKind> AstBuilder::modKinds =
    map<string, ModifiersNode::ModifierKind>(
        {{"public", ModifiersNode::ModifierKind::PUBLIC},
         {"private", ModifiersNode::ModifierKind::PRIVATE},
         {"protected", ModifiersNode::ModifierKind::PROTECTED},
         {"static", ModifiersNode::ModifierKind::STATIC},
         {"final", ModifiersNode::ModifierKind::FINAL},
         {"synchronized", ModifiersNode::ModifierKind::SYNCHRONIZED},
         {"abstract", ModifiersNode::ModifierKind::ABSTRACT},
         {"native", ModifiersNode::ModifierKind::NATIVE}});
