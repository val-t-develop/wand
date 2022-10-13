EXE = spl

setup_gcc:
	CC=gcc CXX=g++ meson setup builddir

setup_clang:
	CC=clang CXX=clang++ meson setup builddir

clean:
	rm -dr builddir build

reload:
	make clean && make setup_gcc

default:
	make build && make run

build:
	cd builddir && meson compile && cd ..

run: 
	echo '\n\n\n' && ./builddir/$(EXE) res/test/test.spl -o res/exe && echo '\n\n\n' && rm ./*.o
