# MemPass

MemPass is an optimization tool to detect memory leaks, use-after-free and double-free errors.  
It's for LLVM 9.

Build:

    $ cd llvm-pass-skeleton
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..

Run:

    $ clang -S -emit-llvm -Xclang -disable-O0-optnone foo.c
    $ opt -load build/skeleton/libSkeletonPass.* -memPass -S foo.ll
    
The `-Xclang -disable-O0-optnone` flag ensures that Clang will allow later optimizations even when initially compiling without any. 
