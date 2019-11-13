#include <stdio.h>
#include <stdint.h>

void logMalloc(int8_t* address, int64_t size) {
    printf("m, %p, %ld\n", address, size);
}

void logFree(int8_t* address) {
    printf("f, %p\n", address);
}

void logQuery(int32_t* address, int64_t size) {
    printf("q, %p, %ld\n", address, size);
}

void logAlloca(int32_t* address) {
    printf("aa, %p\n", address);
}