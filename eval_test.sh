#!/bin/bash
A=$1
B=$2
C=$3

/usr/local/opt/llvm/bin/clang -S -emit-llvm -Wno-everything -Xclang -disable-O0-optnone $A
/usr/local/opt/llvm/bin/clang -S -emit-llvm -Wno-everything -Xclang -disable-O0-optnone rtlib.c
/usr/local/opt/llvm/bin/llvm-link -S $B rtlib.ll -o final.ll
/usr/local/opt/llvm/bin/opt -S -load build/skeleton/libSkeletonPass.* -mempass final.ll -o final_opt.ll
/usr/local/opt/llvm/bin/lli final_opt.ll > $C.csv
