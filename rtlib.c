#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct pair {
    int8_t* address;
    int64_t size;
};

int         allocationIndex;
struct pair allocationMap[1000];

// Compare two <address, size> pairs for equality
bool comparePairs(struct pair pair_one, struct pair pair_two) {
    if (pair_one.address == pair_two.address &&
        pair_one.size == pair_two.size) {
        return true;
    }
    return false;
}

// Initialize data structures with dummy <address, size> pairs
void initialize() {
    allocationIndex = 0;
    struct pair dummyAllocation = {0, 0};
    for (int i = 0; i < 1000; ++i) {
        allocationMap[i] = dummyAllocation;
    }
}

// Log user allocations to the global map
void logMalloc(int8_t* address, int64_t size) {
    struct pair newAllocation = {address, size};
    allocationMap[allocationIndex] = newAllocation;
    allocationIndex++;
}

// Log stack allocations to the global map
void logAlloca(int32_t* address) {
    int8_t* castAddress = (int8_t *)address;
    struct pair newAllocation = {castAddress, 0};
    allocationMap[allocationIndex] = newAllocation;
    allocationIndex++;
}

// Verify that freed memory is allocated in the global map
// Double Free: Freed memory address is missing from the global map
void logFree(int8_t* address, int64_t line, int64_t col) {
    struct pair dummyAllocation = {0, 0};
    for (int i = 0; i < 1000; ++i) {
        if (!comparePairs(allocationMap[i], dummyAllocation) &&
            allocationMap[i].address == address) {
            allocationMap[i] = dummyAllocation;
            return;
        }
    }
    if (line > -1) {
        printf("Line %ld.%ld: Warning! Attempted Double Free\n", line, col);
    }
    else {
        printf("Line ?: Warning! Attempted Double Free\n");
    }
}

// Verify that loads/stores are within the bounds of allocated memory in the global map
// Use After Free: Loads/stores are outside of bounds in global map
void logQuery(int32_t* address, int64_t size, int64_t line, int64_t col) {
    struct pair dummyAllocation = {0, 0};
    int8_t* castAddress = (int8_t *)address;
    int8_t* startAddress = castAddress;
    int8_t* endAddress = castAddress + size;
    int flag = 0;

    for (int i = 0; i < 1000; ++i) {
        if (!comparePairs(allocationMap[i], dummyAllocation)) {
          int8_t* allocStartAddress = allocationMap[i].address;
          int8_t* allocEndAddress = allocationMap[i].address + allocationMap[i].size;

          if (allocationMap[i].size == 0
              && allocStartAddress == startAddress) {
              flag = 2;
              break;
          }
          
          if (allocStartAddress <= startAddress
              && allocEndAddress >= startAddress) {
              flag += 1;
              break;
          }
        }
    }

    for (int i = 0; i < 1000; ++i) {
        if (flag == 2) {
            break;
        }

        if (!comparePairs(allocationMap[i], dummyAllocation)) {
          int8_t* allocStartAddress = allocationMap[i].address;
          int8_t* allocEndAddress = allocationMap[i].address + allocationMap[i].size;

          if (allocStartAddress <= endAddress
              && allocEndAddress >= endAddress) {
              flag += 1;
              break;
          }
        }
    }

    if (flag < 2) {
      if (line > -1) {
        printf("Line %ld.%ld: Warning! Attempted to Use After Free\n", line, col);
      }
      else {
        printf("Line ?: Warning! Attempted to Use After Free\n");
      }
    }
}