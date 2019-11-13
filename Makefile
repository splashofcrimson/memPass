#=============================================================================
# Target rules for targets named MemPass

# Build rule for target.
mempass: example.c rtlib.c skeleton/Skeleton.cpp
	cd ./build && make
	clang-8 -S -emit-llvm -Xclang -disable-O0-optnone example.c
	clang-8 -S -emit-llvm -Xclang -disable-O0-optnone rtlib.c
	llvm-link-8 -S example.ll rtlib.ll -o final.ll
	opt-8 -S -load build/skeleton/libSkeletonPass.* -mempass final.ll -o final_opt.ll
	# rm example.ll rtlib.ll final.ll
	# lli-8 final_opt.ll