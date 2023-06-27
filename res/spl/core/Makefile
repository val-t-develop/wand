lib_c:
	clang -c -S -emit-llvm lib.c -o spl.core.stdlib.ll && sed -i 's/___/./g' spl.core.stdlib.ll