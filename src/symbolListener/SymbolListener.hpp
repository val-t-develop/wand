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

#pragma once
#include <Defs.hpp>
#include <lexer/Lexer.hpp>
#include <symbolTable/SymbolTable.hpp>
#include <utils/Path.hpp>

class SymbolListener {
  public:
    shared_ptr<SymbolTable> symbolTable;
    Path filePath;
    Path srcDir;
    Lexer lexer;
    stack<shared_ptr<ClassRecord>> classes = stack<shared_ptr<ClassRecord>>();
    shared_ptr<ClassRecord> currentClass;
    shared_ptr<MethodRecord> currentMethod;
    bool core = false;

    SymbolListener(shared_ptr<SymbolTable> symbolTable, Path filePath,
                   Lexer lexer);

    void processImport(vector<string> importName);
    void processImportFile(Path& src, shared_ptr<vector<Path>> toImport, vector<string> importName);
    void processImportDir(Path& dir, shared_ptr<vector<Path>> toImport, vector<string> importName);
    void walk();
    void enterPackage();
    void enterImport();
    void enterTypeDecl();
    void enterClassDecl();
    void enterInterfaceDecl();
    void enterEnumDecl();
    string enterGenericDecl();
    string enterGenericTypeDecl();
    void enterClassMemberDecl();
    void enterConstructorDecl(vector<ModifiersNode::ModifierKind> mods);
    void enterDestructorDecl(vector<ModifiersNode::ModifierKind> mods);
    void enterMethodDecl(string type, string id,
                         vector<ModifiersNode::ModifierKind> mods);
    void enterField(string type, string id,
                    vector<ModifiersNode::ModifierKind> mods);
    void enterStatement();
    void enterNotVarStatement();
    void enterLocalVar(vector<ModifiersNode::ModifierKind> mods);
    void enterMethodArgs();
    void enterBlockStatement(bool newScope);
    void enterAssertStatement();
    void enterBreakStatement();
    void enterContinueStatement();
    void enterReturnStatement();
    void enterIfStatement();
    void enterWhileStatement();
    void enterForStatement();
    void enterForEachStatement();
    void enterExpression();
    void enterBinOpRHS(int exprPrec);
    void enterUnOpPrimary();
    void enterPrimary();
    void enterNew();
    void enterParenExpr();
    string enterAccessOrCall(bool arr);
    string enterAccessItem(bool arr);
    void enterArrayAccess();
    string enterGeneric();
    void enterLiteral();
    void enterArrayInitializer();
    // vector<string> enterTypeList();
    // void enterModifiers();
    vector<string> enterExtending();
    string enterType(bool arr);
    vector<ModifiersNode::ModifierKind> enterModifiers();

    static map<string, ModifiersNode::ModifierKind> modKinds;
};
