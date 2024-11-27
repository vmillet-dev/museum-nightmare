#pragma once

#include <atomic>

// Use standard C++17 atomics
#define B2_HAS_ATOMIC 1
#define b2Atomic(T) std::atomic<T>

// Define atomic operations using std::atomic member functions
#define b2AtomicInit(ptr, val) (ptr)->store(val)
#define b2AtomicLoad(ptr) (ptr)->load()
#define b2AtomicStore(ptr, val) (ptr)->store(val)
#define b2AtomicExchange(ptr, val) (ptr)->exchange(val)
#define b2AtomicCompareExchange(ptr, expected, desired) \
    (ptr)->compare_exchange_weak(expected, desired)
#define b2AtomicFetchAdd(ptr, val) (ptr)->fetch_add(val)
#define b2AtomicFetchSub(ptr, val) (ptr)->fetch_sub(val)
