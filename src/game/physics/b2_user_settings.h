#pragma once

#include <atomic>

// Use standard C++17 atomics
#define B2_HAS_ATOMIC 1

// Define atomic operations using std::atomic member functions
#define b2Atomic(T) std::atomic<T>

// Use relaxed memory ordering by default for better performance
#define b2AtomicInit(ptr, val) (ptr)->store(val, std::memory_order_relaxed)
#define b2AtomicLoad(ptr) (ptr)->load(std::memory_order_relaxed)
#define b2AtomicStore(ptr, val) (ptr)->store(val, std::memory_order_relaxed)
#define b2AtomicExchange(ptr, val) (ptr)->exchange(val, std::memory_order_acq_rel)
#define b2AtomicCompareExchange(ptr, expected, desired) \
    (ptr)->compare_exchange_weak(expected, desired, std::memory_order_acq_rel, std::memory_order_relaxed)
#define b2AtomicFetchAdd(ptr, val) (ptr)->fetch_add(val, std::memory_order_acq_rel)
#define b2AtomicFetchSub(ptr, val) (ptr)->fetch_sub(val, std::memory_order_acq_rel)
