project('spl', 'cpp', default_options : 'cpp_std=c++20')
add_project_arguments('-W', '-Wall', '-Wextra', '-Wpedantic', '-g', language : 'cpp')

llvm_dep = dependency('llvm', version : '>=10.0')

incdir = include_directories('src')

src_files = ['src/main.cpp', 'src/main.hpp', 'src/Defs.hpp', 'src/Defs.cpp', 'src/utils/ArgsParser.cpp', 'src/utils/ArgsParser.hpp',
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
    'src/utils/UniqueNumber.hpp', 'src/utils/UniqueNumber.cpp']

executable('spl', src_files, dependencies : llvm_dep, include_directories : incdir)
