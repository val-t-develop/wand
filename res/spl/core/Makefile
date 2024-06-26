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
#  Makefile to generate lib.c llvm ir representation.

lib_c:
	clang -c -S -emit-llvm clib/lib.c -o spl.core.stdlib.ll && sed -i 's/\([^_]\)___\([^_]\)/\1.\2/g' spl.core.stdlib.ll && sed -i '1i;  SPL - Simple Programming Language compiler\
;  Copyright (C) 2022  Valentyn Tymchyshyn\
;\
;  This program is free software: you can redistribute it and/or modify\
;  it under the terms of the GNU General Public License as published by\
;  the Free Software Foundation, either version 3 of the License, or any\
;  later version.\
;\
;  This program is distributed in the hope that it will be useful,\
;  but WITHOUT ANY WARRANTY; without even the implied warranty of\
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\
;  GNU General Public License for more details.\
;\
;  You should have received a copy of the GNU General Public License\
;  along with this program.  If not, see <https://www.gnu.org/licenses/>.\
\
;  Valentyn Tymchyshyn (val.t.develop) (val.t.develo@gmail.com)\
;\
;  Generated lib.c llvm ir representation.\
' spl.core.stdlib.ll