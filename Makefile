#=============================================================================
# Target rules for targets named MemPass

# Build rule for target.
mempass: example.c rtlib.c skeleton/Skeleton.cpp
	cd ./build && make
	clang-8 -S -g -emit-llvm -Xclang -disable-O0-optnone example.c
	clang-8 -S -emit-llvm -Xclang -disable-O0-optnone rtlib.c
	opt-8 -S -load build/skeleton/libSkeletonPass.* -mempass example.ll -o example_opt.ll
	llvm-link-8 -S example_opt.ll rtlib.ll -o final_opt.ll
	rm example.ll rtlib.ll example_opt.ll
	lli-8 final_opt.ll