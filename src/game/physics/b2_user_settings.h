#pragma once

#include <atomic>
#include <cstdlib>

// Enable C++17 features
#define B2_HAS_CPP17 1

// Enable atomic support
#define B2_HAS_ATOMIC 1

// Use standard C++17 atomics for all platforms
#define b2Atomic(T) std::atomic<T>
#define b2AtomicInit(ptr, val) std::atomic_init(ptr, val)
#define b2AtomicLoad(ptr) std::atomic_load_explicit(ptr, std::memory_order_acquire)
#define b2AtomicStore(ptr, val) std::atomic_store_explicit(ptr, val, std::memory_order_release)
#define b2AtomicExchange(ptr, val) std::atomic_exchange_explicit(ptr, val, std::memory_order_acq_rel)
#define b2AtomicCompareExchange(ptr, expected, desired) \
    std::atomic_compare_exchange_strong_explicit(ptr, expected, desired, \
        std::memory_order_acq_rel, std::memory_order_acquire)
#define b2AtomicFetchAdd(ptr, val) std::atomic_fetch_add_explicit(ptr, val, std::memory_order_acq_rel)
#define b2AtomicFetchSub(ptr, val) std::atomic_fetch_sub_explicit(ptr, val, std::memory_order_acq_rel)

// Memory management
#define B2_MEMORY_ALIGNMENT 16
#define B2_ALLOC(size) std::malloc(size)
#define B2_FREE(mem) std::free(mem)
