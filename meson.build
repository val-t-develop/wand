#  SPL - Simple Programming Language compiler
#  Copyright (C) 2022  Valentyn Tymchyshyn
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or any
#  later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#  Valentyn Tymchyshyn (val.t.develop) (val.t.develo@gmail.com)
#
#  Meson.build file for SPL compiler.
#  USE Makefile to compile the project.

project('spl', 'cpp', default_options : 'cpp_std=c++20')
add_project_arguments('-W', '-Wall', '-Wextra', '-Wpedantic', '-g', language : 'cpp')

llvm_dep = dependency('llvm', version : '>18.0.0')

incdir = include_directories('src')

src_files = ['src/main.cpp', 'src/main.hpp', 'src/License.hpp',
    'src/Defs.hpp', 'src/Defs.cpp', 'src/utils/ArgsParser.cpp', 'src/utils/ArgsParser.hpp',
    'src/utils/Out.cpp', 'src/utils/Out.hpp', 'src/utils/Path.cpp', 'src/utils/Path.hpp',
    'src/symbolTable/records/Record.hpp', 'src/symbolTable/records/Record.cpp',
    'src/symbolTable/records/VarRecord.hpp', 'src/symbolTable/records/VarRecord.cpp',
    'src/symbolTable/records/MethodRecord.hpp', 'src/symbolTable/records/MethodRecord.cpp',
    'src/symbolTable/records/ClassRecord.hpp', 'src/symbolTable/records/ClassRecord.cpp',
    'src/symbolTable/Scope.hpp', 'src/symbolTable/Scope.cpp',
    'src/symbolTable/SymbolTable.hpp', 'src/symbolTable/SymbolTable.cpp',
    'src/lexer/Lexer.cpp', 'src/lexer/Lexer.hpp',
    'src/lexer/Token.cpp', 'src/lexer/Token.hpp',
    'src/symbolListener/SymbolListener.hpp', 'src/symbolListener/SymbolListener.cpp',
    'src/utils/ParserUtils.hpp', 'src/utils/ParserUtils.cpp',
    'src/utils/UniqueNumber.hpp', 'src/utils/UniqueNumber.cpp',
    'src/ast/builder/AstBuilder.cpp', 'src/ast/builder/AstBuilder.hpp',
    
    'src/ast/node/VarDeclNode.hpp', 'src/ast/node/VarDeclNode.cpp', 
    'src/ast/node/VarsDeclNode.hpp', 'src/ast/node/VarsDeclNode.cpp',
    'src/ast/node/PackageDeclNode.hpp', 'src/ast/node/PackageDeclNode.cpp', 
    'src/ast/node/Node.hpp', 'src/ast/node/Node.cpp', 
    'src/ast/node/ModifiersNode.hpp', 'src/ast/node/ModifiersNode.cpp', 
    'src/ast/node/MethodDeclNode.hpp', 'src/ast/node/MethodDeclNode.cpp', 
    'src/ast/node/ConstructorDeclNode.hpp', 'src/ast/node/ConstructorDeclNode.cpp',
    'src/ast/node/DestructorDeclNode.hpp', 'src/ast/node/DestructorDeclNode.cpp',
    'src/ast/node/ImportDeclNode.hpp', 'src/ast/node/ImportDeclNode.cpp',
    'src/ast/node/GenericNode.hpp', 'src/ast/node/GenericNode.cpp', 
    'src/ast/node/CompilationUnitNode.hpp', 'src/ast/node/CompilationUnitNode.cpp', 
    'src/ast/node/ClassRecordNode.hpp', 'src/ast/node/ClassRecordNode.cpp', 
    'src/ast/node/ClassDeclNode.hpp', 'src/ast/node/ClassDeclNode.cpp', 
    'src/ast/node/AccessNode.hpp', 'src/ast/node/AccessNode.cpp', 
    'src/ast/node/statement/WhileNode.hpp', 'src/ast/node/statement/WhileNode.cpp', 
    'src/ast/node/statement/StatementNode.hpp', 'src/ast/node/statement/StatementNode.cpp', 
    'src/ast/node/statement/ReturnNode.hpp', 'src/ast/node/statement/ReturnNode.cpp', 
    'src/ast/node/statement/IfElseNode.hpp', 'src/ast/node/statement/IfElseNode.cpp', 
    'src/ast/node/statement/ForNode.hpp', 'src/ast/node/statement/ForNode.cpp', 
    'src/ast/node/statement/ForEachNode.hpp', 'src/ast/node/statement/ForEachNode.cpp', 
    'src/ast/node/statement/ContinueNode.hpp', 'src/ast/node/statement/ContinueNode.cpp', 
    'src/ast/node/statement/BreakNode.hpp', 'src/ast/node/statement/BreakNode.cpp', 
    'src/ast/node/statement/BlockNode.hpp', 'src/ast/node/statement/BlockNode.cpp', 
    'src/ast/node/statement/AssertNode.hpp', 'src/ast/node/statement/AssertNode.cpp', 
    'src/ast/node/statement/expression/VarRecordNode.hpp', 'src/ast/node/statement/expression/VarRecordNode.cpp', 
    'src/ast/node/statement/expression/UnaryOperatorNode.hpp', 'src/ast/node/statement/expression/UnaryOperatorNode.cpp', 
    'src/ast/node/statement/expression/TypeNode.hpp', 'src/ast/node/statement/expression/TypeNode.cpp', 
    'src/ast/node/statement/expression/NewNode.hpp', 'src/ast/node/statement/expression/NewNode.cpp', 
    'src/ast/node/statement/expression/MethodCallNode.hpp', 'src/ast/node/statement/expression/MethodCallNode.cpp', 
    'src/ast/node/statement/expression/ExpressionNode.hpp', 'src/ast/node/statement/expression/ExpressionNode.cpp',
    'src/ast/node/statement/expression/CastExpr.hpp', 'src/ast/node/statement/expression/CastExpr.cpp', 
    'src/ast/node/statement/expression/BinaryOperatorNode.hpp', 'src/ast/node/statement/expression/BinaryOperatorNode.cpp', 
    'src/ast/node/statement/expression/AssignableNode.hpp', 'src/ast/node/statement/expression/AssignableNode.cpp', 
    'src/ast/node/statement/expression/ArrayInitializerNode.hpp', 'src/ast/node/statement/expression/ArrayInitializerNode.cpp', 
    'src/ast/node/statement/expression/ArrayCreationNode.hpp', 'src/ast/node/statement/expression/ArrayCreationNode.cpp', 
    'src/ast/node/statement/expression/ArrayAccessNode.hpp', 'src/ast/node/statement/expression/ArrayAccessNode.cpp', 
    'src/ast/node/statement/expression/literal/StringLiteralNode.hpp', 'src/ast/node/statement/expression/literal/StringLiteralNode.cpp', 
    'src/ast/node/statement/expression/literal/NullNode.hpp', 'src/ast/node/statement/expression/literal/NullNode.cpp', 
    'src/ast/node/statement/expression/literal/IntLiteralNode.hpp', 'src/ast/node/statement/expression/literal/IntLiteralNode.cpp', 
    'src/ast/node/statement/expression/literal/FloatLiteralNode.hpp', 'src/ast/node/statement/expression/literal/FloatLiteralNode.cpp', 
    'src/ast/node/statement/expression/literal/CharLiteralNode.hpp', 'src/ast/node/statement/expression/literal/CharLiteralNode.cpp', 
    'src/ast/node/statement/expression/literal/BoolLiteralNode.hpp', 'src/ast/node/statement/expression/literal/BoolLiteralNode.cpp',
    'src/IRTree/builder/IRTreeBuilder.hpp', 'src/IRTree/builder/IRTreeBuilder.cpp',
    'src/IRTree/node/IRNode.hpp', 'src/IRTree/node/IRNode.cpp',
    'src/IRTree/node/IRTree.hpp', 'src/IRTree/node/IRTree.cpp',
    'src/IRTree/node/IRFunction.hpp', 'src/IRTree/node/IRFunction.cpp',
    'src/IRTree/node/IRStruct.hpp', 'src/IRTree/node/IRStruct.cpp',
    'src/IRTree/node/statement/IRStatement.hpp', 'src/IRTree/node/statement/IRStatement.cpp',
    'src/IRTree/node/statement/IRBlock.hpp', 'src/IRTree/node/statement/IRBlock.cpp',
    'src/IRTree/node/statement/IRReturn.hpp', 'src/IRTree/node/statement/IRReturn.cpp',
    'src/IRTree/node/statement/IRIfElse.hpp', 'src/IRTree/node/statement/IRIfElse.cpp',
    'src/IRTree/node/statement/IRVarDecl.hpp', 'src/IRTree/node/statement/IRVarDecl.cpp',
    'src/IRTree/node/statement/IRWhile.hpp', 'src/IRTree/node/statement/IRWhile.cpp',
    'src/IRTree/node/statement/expression/IRExpression.hpp', 'src/IRTree/node/statement/expression/IRExpression.cpp',
    'src/IRTree/node/statement/expression/IRCall.hpp', 'src/IRTree/node/statement/expression/IRCall.cpp',
    'src/IRTree/node/statement/expression/IRBinOp.hpp', 'src/IRTree/node/statement/expression/IRBinOp.cpp',
    'src/IRTree/node/statement/expression/IRUnOp.hpp', 'src/IRTree/node/statement/expression/IRUnOp.cpp',
    'src/IRTree/node/statement/expression/IRLiteral.hpp', 'src/IRTree/node/statement/expression/IRLiteral.cpp',
    'src/IRTree/node/statement/expression/IRVar.hpp', 'src/IRTree/node/statement/expression/IRVar.cpp',
    'src/IRTree/node/statement/expression/IRAccess.hpp', 'src/IRTree/node/statement/expression/IRAccess.cpp',
    'src/IRTree/node/statement/expression/IRAlloc.hpp', 'src/IRTree/node/statement/expression/IRAlloc.cpp',

    'src/codeGen/utils/LLVMHelper.cpp', 'src/codeGen/utils/LLVMHelper.hpp']

executable('spl', src_files, dependencies : llvm_dep, include_directories : incdir)
