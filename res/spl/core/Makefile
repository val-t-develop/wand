lib_c:
	clang -c -S -emit-llvm lib.c -o lib.ll && sed -i 's/___/./g' lib.ll