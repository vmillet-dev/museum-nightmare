#pragma once

#include <atomic>
#include <cstdlib>

// Enable C++17 features
#define B2_HAS_CPP17 1

// Use standard C++17 atomics
#define B2_HAS_ATOMIC 1

// Define atomic operations using std::atomic
template<typename T>
using b2AtomicType = std::atomic<T>;

#define b2Atomic(T) b2AtomicType<T>
#define b2AtomicInit(ptr, val) std::atomic_init(ptr, val)
#define b2AtomicLoad(ptr) (ptr)->load(std::memory_order_acquire)
#define b2AtomicStore(ptr, val) (ptr)->store(val, std::memory_order_release)
#define b2AtomicExchange(ptr, val) (ptr)->exchange(val, std::memory_order_acq_rel)
#define b2AtomicCompareExchange(ptr, expected, desired) \
    (ptr)->compare_exchange_strong(expected, desired, std::memory_order_acq_rel)
#define b2AtomicFetchAdd(ptr, val) (ptr)->fetch_add(val, std::memory_order_acq_rel)
#define b2AtomicFetchSub(ptr, val) (ptr)->fetch_sub(val, std::memory_order_acq_rel)

// Memory management
#define B2_MEMORY_ALIGNMENT 16
#define B2_ALLOC(size) std::malloc(size)
#define B2_FREE(mem) std::free(mem)
