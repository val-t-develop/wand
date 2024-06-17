EXE = spl

setup_gcc:
	CC=gcc CXX=g++ meson setup builddir && cp builddir/compile_commands.json compile_commands.json

setup_clang:
	CC=clang CXX=clang++ meson setup builddir && cp builddir/compile_commands.json compile_commands.json

clean:
	rm -dr builddir build compile_commands.json

reload:
	make clean && make setup_gcc

default:
	make build && make run

build:
	cd builddir && meson compile && cd ..

run: 
	echo '\n\n\n' && ./builddir/$(EXE) res -o res/exe && echo '\n\n\n'
