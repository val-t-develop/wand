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
#  File for setting up, building and running SPL compiler.

EXE = spl

setup_gcc:
	CC=gcc CXX=g++ meson setup builddir && cp builddir/compile_commands.json compile_commands.json

setup_clang:
	CC=clang CXX=clang++ meson setup builddir && cp builddir/compile_commands.json compile_commands.json

clean:
	rm -dr builddir compile_commands.json

reload:
	make clean && make setup_gcc

default:
	make build && make run

build:
	cd builddir && meson compile && cd ..

run: 
	echo '\n\n\n' && ./builddir/$(EXE) res -o res/exe && echo '\n\n\n'
