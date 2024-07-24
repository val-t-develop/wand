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
 *  Builder of ir tree.
 */

#include "IRTreeBuilder.hpp"

#include <IRTree/node/statement/IRIfElse.hpp>
#include <IRTree/node/statement/IRReturn.hpp>
#include <IRTree/node/statement/IRVarsDecl.hpp>
#include <IRTree/node/statement/IRWhile.hpp>
#include <IRTree/node/statement/expression/IRAccess.hpp>
#include <IRTree/node/statement/expression/IRAlloc.hpp>
#include <IRTree/node/statement/expression/IRFunc.hpp>
#include <IRTree/node/statement/expression/IRVar.hpp>
#include <ast/node/statement/expression/literal/BoolLiteralNode.hpp>
#include <ast/node/statement/expression/literal/CharLiteralNode.hpp>
#include <ast/node/statement/expression/literal/FloatLiteralNode.hpp>
#include <ast/node/statement/expression/literal/IntLiteralNode.hpp>
#include <ast/node/statement/expression/literal/StringLiteralNode.hpp>
#include <main.hpp>

IRTreeBuilder::IRTreeBuilder(shared_ptr<CompilationUnitNode> _cu, Path &_file)
    : cu(_cu), tree(make_shared<IRTree>()), file(_file) {}

void IRTreeBuilder::walk() {
    enterImports(file);

    for (auto ps : imported) {
        Path p(ps);
        auto importCU = Main::CUs[p]->cu;

        for (shared_ptr<Node> n : importCU->nodes) {
            if (n->kind == Node::NodeKind::PACKAGE_DECL_NODE) {
                auto name = static_pointer_cast<PackageDeclNode>(n)->name;

                tree->moduleName = "";
                for (string str :
                     static_pointer_cast<PackageDeclNode>(cu->nodes[0])->name) {
                    if (str != "") {
                        tree->moduleName += str + ".";
                    }
                     }
                tree->moduleName.pop_back();


                if (name.size()==2) {
                    if (name[0]=="spl" && name[1]=="core") {
                        if (p.getFilename() == "core.spl") {
                            auto dir = p.getParent();
                            auto ll_file = Path(dir.getName() + "/spl.core.stdlib.ll");
                            system(string("mkdir -p .spl_compilation"+dir.getName()).c_str());
                            string o_file = ".spl_compilation" + dir.getName() + "/spl.core.stdlib.o";
                            Main::currCUsStack.top()->linkingObj.push_back(o_file);
                            system(string("clang " + ll_file.getName() + " -c -o " + o_file).c_str());
                        }
                    }
                }
            } else if (n->kind == Node::NodeKind::IMPORT_DECL_NODE) {
                //enterImports(p); // TODO
            } else if (n->kind == Node::NodeKind::CLASS_DECL_NODE) {
                enterClassDecl(static_pointer_cast<ClassDeclNode>(n), false);
            } else {
                Out::errorMessage("Can not generate this node");
            }
        }
    }

    for (shared_ptr<Node> node : cu->nodes) {
        if (node->kind == Node::NodeKind::CLASS_DECL_NODE) {
            enterClassDecl(static_pointer_cast<ClassDeclNode>(node), true);
        }
    }
}

void IRTreeBuilder::enterImports(Path f) {
    vector<Path> importFiles{};
    for (auto imports : Main::CUs[f]->importFiles) {
        auto paths = imports.second;
        for (auto path : paths) {
            importFiles.push_back(path);
        }
    }

    for (auto p : importFiles) {
        bool alreadyImported = false;
        for (string importedOne : imported) {
            if (importedOne == p.getName()) {
                alreadyImported = true;
            }
        }
        if (alreadyImported) {
            continue;
        }
        imported.push_back(p.getName());
        Main::CUs[p]->completeToState(CU::State::AST);
    }
}

void IRTreeBuilder::enterClassDecl(shared_ptr<ClassDeclNode> node,
                                   bool genMethods) {
    classesStack.push(node);
    shared_ptr<IRStruct> Struct =
        make_shared<IRStruct>(node->getFullName(), vector<shared_ptr<IRVarDecl>>{});
    for (auto el : node->innerClasses) {
        enterClassDecl(el, genMethods);
    }

    for (auto el : node->fields) {
        bool isStatic = false;
        for (auto mod : el->modifiers->modifiers) {
            if (mod == ModifiersNode::ModifierKind::STATIC) {
                isStatic = true;
                break;
            }
        }
        if (isStatic) {
            tree->globalVars.push_back(make_shared<IRVarDecl>(el->getFullName(), el->type->getFullName(), nullptr));
            GlobalNamedValues.push_back(el->getFullName());
        } else {
            Struct->fields.push_back(make_shared<IRVarDecl>(el->getFullName(), el->type->getFullName(), nullptr));
        }
    }
    tree->structs.push_back(Struct);
    if (genMethods) {
        for (auto el : node->methods) {
            enterMethod(el);
        }
        for (auto el : node->constructors) {
            enterConstructor(el, false);
        }
        if (node->constructors.empty()) {
            enterConstructor(make_shared<ConstructorDeclNode>(
                nullptr, nullptr, vector<shared_ptr<VarDeclNode>>{}, nullptr,
                nullptr), true);
        }
        enterDestructor(node);
    } else {
        for (auto el : node->methods) {
            for (auto el : node->methods) {
                enterMethodPrototype(el);
            }
        }
    }
    classesStack.pop();
}

void IRTreeBuilder::enterMethod(shared_ptr<MethodDeclNode> node) {
    vector<shared_ptr<IRVarDecl>> args{};
    shared_ptr<IRBlock> body = nullptr;

    NamedValues.clear();
    for (auto arg : node->args) {
        args.push_back(make_shared<IRVarDecl>(arg->getFullName(),
                                              arg->type->getFullName(), nullptr));
        NamedValues.push_back(arg->getFullName());
    }
    if (node->body != nullptr) {
        body = enterBlock(node->body);
    }
    tree->funcs[node->getFullName()] = make_shared<IRFunction>(
        node->getFullName(), node->returnType->getFullName(), args, body);
}

void IRTreeBuilder::enterMethodPrototype(shared_ptr<MethodDeclNode> node) {
    vector<shared_ptr<IRVarDecl>> args{};

    for (auto arg : node->args) {
        args.push_back(make_shared<IRVarDecl>(arg->getFullName(),
                                              arg->type->getFullName(), nullptr));
    }
    tree->funcs[node->getFullName()] = make_shared<IRFunction>(
        node->getFullName(), node->returnType->getFullName(), args, nullptr);
}

void IRTreeBuilder::enterConstructor(shared_ptr<ConstructorDeclNode> node,
                                     bool defaultConstrucctor) {
    vector<shared_ptr<IRVarDecl>> args{};
    string argsSpec = "";
    shared_ptr<IRBlock> body = nullptr;

    for (auto arg : node->args) {
        args.push_back(make_shared<IRVarDecl>(arg->getFullName(),
                                              arg->type->getFullName(), nullptr));
        argsSpec += "__" + arg->type->getFullName();
    }
    if (node->body != nullptr) {
        body = enterBlock(node->body);
    }
    if (defaultConstrucctor) {
        body=make_shared<IRBlock>(vector<shared_ptr<IRStatement>>());
    }
    vector<shared_ptr<IRStatement>> constructorHeader{};
    constructorHeader.push_back(make_shared<IRVarDecl>("this", classesStack.top()->getFullName(), make_shared<IRAlloc>(classesStack.top()->getFullName())));
    for (auto el : classesStack.top()->fields) {
        auto access = make_shared<IRAccess>();
        access->access.push_back(make_shared<IRVar>("this"));
        access->access.push_back(make_shared<IRVar>(el->getFullName()));
        constructorHeader.push_back(make_shared<IRBinOp>(access, getDefaultValue(el->type->getFullName()), BinaryOperatorNode::BinaryOperatorKind::ASSIGN));
    }
    if (body!=nullptr) {
        body->nodes.insert(body->nodes.begin(), constructorHeader.begin(), constructorHeader.end());
        body->nodes.push_back(make_shared<IRReturn>(make_shared<IRVar>("this")));
    }
    auto currClass = classesStack.top();
    string name = "__spl__constructor__" + currClass->getFullName() + argsSpec;
    tree->funcs[name] = make_shared<IRFunction>(name, currClass->getFullName(), args, body);
}

void IRTreeBuilder::enterDestructor(shared_ptr<ClassDeclNode> node) {
    shared_ptr<IRBlock> body = nullptr;
    if (!node->destructors.empty()) {
        if (node->destructors[0]->body != nullptr) {
            body = enterBlock(node->destructors[0]->body);
        }
    } else {
        body = make_shared<IRBlock>(vector<shared_ptr<IRStatement>>{});
    }
    for (auto el : classesStack.top()->fields) {
        auto access = make_shared<IRAccess>();
        access->access.push_back(make_shared<IRVar>("this"));
        access->access.push_back(make_shared<IRVar>(el->getFullName()));
        if (el->type->type->record->type!="primitive") {
            body->nodes.push_back(make_shared<IRCall>("__spl__destroyref", vector<shared_ptr<IRExpression>>{access, make_shared<IRFunc>("__spl__destructor__"+el->type->getFullName())}));
        }
    }
    auto currClass = classesStack.top();
    string name = "__spl__destructor__" + currClass->getFullName();
    tree->funcs[name] = make_shared<IRFunction>(name, "void",
        vector<shared_ptr<IRVarDecl>>{
            make_shared<IRVarDecl>("this", currClass->getFullName(), nullptr)},
        body);
}

shared_ptr<IRBlock> IRTreeBuilder::enterBlock(shared_ptr<BlockNode> node) {
    shared_ptr<IRBlock> block =
        make_shared<IRBlock>(vector<shared_ptr<IRStatement>>{});
    for (auto el : node->nodes) {
        if (el->kind == Node::NodeKind::BLOCK_NODE) {
            block->nodes.push_back(enterBlock(static_pointer_cast<BlockNode>(el)));
        } else if (el->kind == Node::NodeKind::RETURN_NODE) {
            block->nodes.push_back(make_shared<IRReturn>(enterExpression(static_pointer_cast<ReturnNode>(el)->expression)));
        } else if (el->kind == Node::NodeKind::VAR_DECL_NODE) {
            auto varDeclNode = static_pointer_cast<VarDeclNode>(el);
            NamedValues.push_back(varDeclNode->getFullName());
            block->nodes.push_back(make_shared<IRVarDecl>(varDeclNode->getFullName(), varDeclNode->type->getFullName(), varDeclNode->init!=nullptr?enterExpression(varDeclNode->init):nullptr));
        } else if (el->kind == Node::NodeKind::VARS_DECL_NODE) {
            for (auto varDeclNode : static_pointer_cast<VarsDeclNode>(el)->decls) {
                NamedValues.push_back(varDeclNode->getFullName());
                block->nodes.push_back(make_shared<IRVarDecl>(varDeclNode->getFullName(), varDeclNode->type->getFullName(), varDeclNode->init!=nullptr?enterExpression(varDeclNode->init):nullptr));
            }
        } else if (el->kind == Node::NodeKind::IF_ELSE_NODE) {
            auto ifElseNode = static_pointer_cast<IfElseNode>(el);
            block->nodes.push_back(make_shared<IRIfElse>(enterExpression(ifElseNode->condition), enterStatement(ifElseNode->thenNode), enterStatement(ifElseNode->elseNode)));
        } else if (el->kind == Node::NodeKind::WHILE_NODE) {
            auto whileNode = static_pointer_cast<WhileNode>(el);
            block->nodes.push_back(make_shared<IRWhile>(vector<shared_ptr<IRStatement>>{}, enterExpression(whileNode->expression), enterStatement(whileNode->statement)));
        } else if (el->kind == Node::NodeKind::FOR_NODE) {
            // TODO for loop
        } else if (el->isExpression()) {
            block->nodes.push_back(enterExpression(static_pointer_cast<ExpressionNode>(el)));
        }
    }
    return block;
}
shared_ptr<IRStatement>
IRTreeBuilder::enterStatement(shared_ptr<StatementNode> el) {
    if (el->kind == Node::NodeKind::BLOCK_NODE) {
        return enterBlock(static_pointer_cast<BlockNode>(el));
    } else if (el->kind == Node::NodeKind::RETURN_NODE) {
        return make_shared<IRReturn>(
            enterExpression(static_pointer_cast<ReturnNode>(el)->expression));
    } else if (el->kind == Node::NodeKind::VAR_DECL_NODE) {
        auto varDeclNode = static_pointer_cast<VarDeclNode>(el);
        NamedValues.push_back(varDeclNode->getFullName());
        return make_shared<IRVarDecl>(varDeclNode->getFullName(), varDeclNode->type->getFullName(), varDeclNode->init!=nullptr?enterExpression(varDeclNode->init):nullptr);
    } else if (el->kind == Node::NodeKind::VARS_DECL_NODE) {
        shared_ptr<IRVarsDecl> vars = make_shared<IRVarsDecl>();
        for (auto varDeclNode : static_pointer_cast<VarsDeclNode>(el)->decls) {
            NamedValues.push_back(varDeclNode->getFullName());
            vars->vars.push_back(make_shared<IRVarDecl>(varDeclNode->getFullName(), varDeclNode->type->getFullName(), varDeclNode->init!=nullptr?enterExpression(varDeclNode->init):nullptr));
        }
        return vars;
    } else if (el->kind == Node::NodeKind::IF_ELSE_NODE) {
        auto ifElseNode = static_pointer_cast<IfElseNode>(el);
        return make_shared<IRIfElse>(enterExpression(ifElseNode->condition),
                                     enterStatement(ifElseNode->thenNode),
                                     enterStatement(ifElseNode->elseNode));
    } else if (el->kind == Node::NodeKind::WHILE_NODE) {
        auto whileNode = static_pointer_cast<WhileNode>(el);
        return make_shared<IRWhile>(vector<shared_ptr<IRStatement>>{},
                                    enterExpression(whileNode->expression),
                                    enterStatement(whileNode->statement));
    } else if (el->kind == Node::NodeKind::FOR_NODE) {
        // TODO for loop
    } else if (el->isExpression()) {
        return enterExpression(static_pointer_cast<ExpressionNode>(el));
    }
}
shared_ptr<IRExpression>
IRTreeBuilder::enterExpression(shared_ptr<ExpressionNode> node) {
    if (node->isLiteral()) {
        return enterLiteral(node);
    } else if (node->kind == Node::NodeKind::METHOD_CALL_NODE) {
        shared_ptr<MethodCallNode> callNode = static_pointer_cast<MethodCallNode>(node);
        bool isStatic = false;
        for (auto mod : callNode->record->mods) {
            if (mod == ModifiersNode::ModifierKind::STATIC) {
                isStatic = true;
                break;
            }
        }
        if (isStatic) {
            return enterCall(callNode);
        } else {
            callNode->args.insert(callNode->args.begin(), make_shared<VarRecordNode>(make_shared<VarRecord>("this", classesStack.top()->getFullName(), Record::RecordKind::LOCAL_VAR_RECORD), nullptr));
            return enterCall(callNode);
        }
    } else if (node->kind == Node::NodeKind::BINARY_OPERATOR_NODE) {
        return enterBinOp(static_pointer_cast<BinaryOperatorNode>(node));
    } else if (node->kind == Node::NodeKind::UNARY_OPERATOR_NODE) {
        // TODO
    } else if (node->kind == Node::NodeKind::VAR_RECORD_NODE) {
        bool exists = false;
        string name = static_pointer_cast<VarRecordNode>(node)->getFullName();
        for (auto el : NamedValues) {
            if (el == name) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            for (auto el : GlobalNamedValues) {
                if (el == name) {
                    exists = true;
                    break;
                }
            }
        }
        if (exists) {
            return make_shared<IRVar>(name);
        } else {
            for (auto el : classesStack.top()->fields) {
                if (el->getFullName() == name) {
                    exists = true;
                    break;
                }
            }
            if (exists) {
                auto access = make_shared<IRAccess>();
                access->access.push_back(make_shared<IRVar>("this"));
                access->access.push_back(make_shared<IRVar>(name));
                return access;
            }
        }
    } else if (node->kind == Node::NodeKind::NEW_NODE) {
        return enterNew(static_pointer_cast<NewNode>(node));
    } else if (node->kind == Node::NodeKind::ARRAY_CREATION_NODE) {
        // TODO
    } else if (node->kind == Node::NodeKind::ARRAY_INITIALIZER_NODE) {
        // TODO
    } else if (node->kind == Node::NodeKind::ARRAY_ACCESS_NODE) {
        // TODO
    } else if (node->kind == Node::NodeKind::ACCESS_NODE) {
        auto access = static_pointer_cast<AccessNode>(node);
        auto access1 = make_shared<AccessNode>(node);
        auto new_access = make_shared<IRAccess>();
        for (auto el : access->access) {
            if (!el->isExpression()) {
                Out::errorMessage("Can not generate expression");
            }
            auto expr = static_pointer_cast<ExpressionNode>(el);
            if (el->kind==Node::NodeKind::METHOD_CALL_NODE) {
                auto call = static_pointer_cast<MethodCallNode>(expr);
                if (!access1->access.empty()) {
                    call->args.insert(call->args.begin(), access1);
                    auto call_ir = enterCall(call);
                    new_access->access=vector<shared_ptr<IRExpression>>();
                    new_access->access.push_back(call_ir);
                } else {
                    new_access->access.push_back(enterCall(call));
                }
            } else if (el->kind==Node::NodeKind::VAR_RECORD_NODE) {
                auto rec = static_pointer_cast<VarRecordNode>(el);
                if (new_access->access.empty()) {
                    new_access->access.push_back(enterExpression(expr));
                } else {
                    bool found = false;
                    for (auto f : access1->getReturnType()->fields) {
                        if (f->getFullName()==rec->getFullName()) {
                            found=true;
                            break;
                        }
                    }
                    if (found) {
                        new_access->access.push_back(make_shared<IRVar>(rec->getFullName()));
                    } else {
                        Out::errorMessage("Can not find field " + rec->getFullName() + " in class " + access1->getReturnType()->getFullName());
                    }
                }
            } else {
                new_access->access.push_back(enterExpression(expr));
            }
            access1->access.push_back(el);
        }
        return new_access;
    }
    return nullptr;
}

shared_ptr<IRLiteral>
IRTreeBuilder::enterLiteral(shared_ptr<ExpressionNode> node) {
    shared_ptr<IRLiteral> literal = nullptr;
    if (node->kind == Node::NodeKind::INT_LITERAL_NODE) {
        auto IntLiteral = static_pointer_cast<IntLiteralNode>(node);
        if (IntLiteral->type == IntLiteralNode::Type::BYTE) {
            literal = make_shared<IRLiteral>(IRNode::Kind::BYTE_LITERAL);
        } else if (IntLiteral->type == IntLiteralNode::Type::SHORT) {
            literal = make_shared<IRLiteral>(IRNode::Kind::SHORT_LITERAL);
        } else if (IntLiteral->type == IntLiteralNode::Type::INT) {
            literal = make_shared<IRLiteral>(IRNode::Kind::INT_LITERAL);
        } else if (IntLiteral->type == IntLiteralNode::Type::LONG) {
            literal = make_shared<IRLiteral>(IRNode::Kind::LONG_LITERAL);
        }
        literal->intLoteral = IntLiteral->value;
    } else if (node->kind == Node::NodeKind::BOOL_LITERAL_NODE) {
        literal = make_shared<IRLiteral>(IRNode::Kind::BOOL_LITERAL);
        literal->boolLiteral =
            static_pointer_cast<BoolLiteralNode>(node)->value;
    } else if (node->kind == Node::NodeKind::CHAR_LITERAL_NODE) {
        literal = make_shared<IRLiteral>(IRNode::Kind::CHAR_LITERAL);
        literal->strLiteral = static_pointer_cast<CharLiteralNode>(node)->str;
    } else if (node->kind == Node::NodeKind::FLOAT_LITERAL_NODE) {
        if (static_pointer_cast<FloatLiteralNode>(node)->doubleVal) {
            literal = make_shared<IRLiteral>(IRNode::Kind::DOUBLE_LITERAL);
            literal->doubleLiteral =
                static_pointer_cast<FloatLiteralNode>(node)->value;
        } else {
            literal = make_shared<IRLiteral>(IRNode::Kind::FLOAT_LITERAL);
            literal->doubleLiteral =
                static_pointer_cast<FloatLiteralNode>(node)->value;
        }
    } else if (node->kind == Node::NodeKind::STRING_LITERAL_NODE) {
        literal = make_shared<IRLiteral>(IRNode::Kind::STRING_LITERAL);
        literal->strLiteral = static_pointer_cast<StringLiteralNode>(node)->str;
    } else if (node->kind == Node::NodeKind::NULL_LITERAL_NODE) {
        literal = make_shared<IRLiteral>(IRNode::Kind::NULL_LITERAL);
    }
    return literal;
}

shared_ptr<IRCall> IRTreeBuilder::enterCall(shared_ptr<MethodCallNode> node) {
    if (!node->record->similar.empty()) {
        bool found = false;
        for (auto el : node->record->similar) {
            if (el->argsCount==node->args.size()) {
                bool equal = true;
                for (int i = 0; i < node->args.size(); ++i) {
                    if (el->vars[i]->typeRec->getFullName()!=node->args[i]->getReturnType()->getFullName()) {
                        equal=false;
                    }
                }
                if (equal) {
                    if (found) {
                        Out::errorMessage("Few function canditates are possible");
                    }
                    node->record=el;
                    found=true;
                }
            }
        }
        if (!found) {
            Out::errorMessage("Can not find function with such arguments");
        }
    }
    vector<shared_ptr<IRExpression>> args{};
    for (auto el : node->args) {
        args.push_back(enterExpression(el));
    }
    return make_shared<IRCall>(node->getFullName(), args);
}

shared_ptr<IRBinOp>
IRTreeBuilder::enterBinOp(shared_ptr<BinaryOperatorNode> node) {
    auto L = enterExpression(node->left);
    auto R = enterExpression(node->right);

    if (node->op ==
        BinaryOperatorNode::BinaryOperatorKind::RIGHT_SHIFT_ASSIGN) {

    } else if (node->op ==
               BinaryOperatorNode::BinaryOperatorKind::LEFT_SHIFT_ASSIGN) {

    } else if (node->op ==
               BinaryOperatorNode::BinaryOperatorKind::BIT_OR_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::XOR_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MOD_ASSIGN) {

    } else if (node->op ==
               BinaryOperatorNode::BinaryOperatorKind::BIT_AND_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::DIV_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MUL_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::SUB_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::ADD_ASSIGN) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::ASSIGN) {
        return make_shared<IRBinOp>(L, R, node->op);
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::OR) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::AND) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::BIT_OR) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::XOR) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::BIT_AND) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::EQUAL) {
        return make_shared<IRBinOp>(L, R, node->op);
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::NOT_EQUAL) {
        return make_shared<IRBinOp>(L, R, node->op);
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LESS) {
        return make_shared<IRBinOp>(L, R, node->op);
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::GREATER) {
        return make_shared<IRBinOp>(L, R, node->op);
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LESS_EQUAL) {
        return make_shared<IRBinOp>(L, R, node->op);
    } else if (node->op ==
               BinaryOperatorNode::BinaryOperatorKind::GREATER_EQUAL) {
        return make_shared<IRBinOp>(L, R, node->op);
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::INSTANCEOF) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::LEFT_SHIFT) {

    } else if (node->op ==
               BinaryOperatorNode::BinaryOperatorKind::RIGHT_SHIFT) {

    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::ADD) {
        return make_shared<IRBinOp>(L, R,
                                    node->op); // TODO String concat with +
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::SUB) {
        return make_shared<IRBinOp>(L, R, node->op);
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MUL) {
        return make_shared<IRBinOp>(L, R, node->op);
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::DIV) {
        return make_shared<IRBinOp>(L, R, node->op);
    } else if (node->op == BinaryOperatorNode::BinaryOperatorKind::MOD) {
    }
    Out::errorMessage("BUG! Can not return value for binary operation");
    return nullptr;
}

shared_ptr<IRCall> IRTreeBuilder::enterNew(shared_ptr<NewNode> node) {
    vector<shared_ptr<IRExpression>> args{};
    string argsSpec = node->type->getFullName();
    for (auto el : node->args) {
        args.push_back(enterExpression(el));
        argsSpec += "__" + el->getReturnType()->getFullName();
    }
    return make_shared<IRCall>("__spl__constructor__" + argsSpec, args);
}

shared_ptr<IRExpression>
IRTreeBuilder::getDefaultValue(string type) {
    if (type=="char") {
        return make_shared<IRLiteral>(IRNode::Kind::CHAR_LITERAL);
    } else if (type=="bool") {
        return make_shared<IRLiteral>(IRNode::Kind::BOOL_LITERAL);
    } else if (type=="byte") {
        return make_shared<IRLiteral>(IRNode::Kind::BYTE_LITERAL);
    } else if (type=="short") {
        return make_shared<IRLiteral>(IRNode::Kind::SHORT_LITERAL);
    } else if (type=="int") {
        return make_shared<IRLiteral>(IRNode::Kind::INT_LITERAL);
    } else if (type=="long") {
        return make_shared<IRLiteral>(IRNode::Kind::LONG_LITERAL);
    } else if (type=="float") {
        return make_shared<IRLiteral>(IRNode::Kind::FLOAT_LITERAL);
    } else if (type=="double") {
        return make_shared<IRLiteral>(IRNode::Kind::DOUBLE_LITERAL);
    } else if (type=="String") {
        return make_shared<IRLiteral>(IRNode::Kind::STRING_LITERAL);
    } else {
        return make_shared<IRLiteral>(IRNode::Kind::NULL_LITERAL);
    }
}