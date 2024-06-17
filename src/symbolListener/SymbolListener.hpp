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
