#pragma once
#include <stdlib.h>
constexpr int MALLOC_CAP_SPIRAM=1,MALLOC_CAP_8BIT=2,MALLOC_CAP_INTERNAL=4;
inline bool hostAllocationFails=false;
inline bool hostPSRAMUnavailable=false;
inline void *heap_caps_malloc(size_t bytes,int caps) { return (hostAllocationFails || (hostPSRAMUnavailable && (caps&MALLOC_CAP_SPIRAM)))?nullptr:malloc(bytes); }
inline size_t heap_caps_get_free_size(int caps) { return (caps&MALLOC_CAP_SPIRAM)?(hostPSRAMUnavailable?0:8*1024*1024):96*1024; }
inline size_t heap_caps_get_largest_free_block(int) { return 80*1024; }
