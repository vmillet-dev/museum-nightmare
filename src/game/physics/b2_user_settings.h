#pragma once

#include <atomic>

#ifdef _MSC_VER
    // MSVC-specific atomic configuration
    #define B2_HAS_ATOMIC 1
    #define b2Atomic(T) std::atomic<T>
    #define b2AtomicInit(ptr, val) (ptr)->store(val)
    #define b2AtomicLoad(ptr) (ptr)->load()
    #define b2AtomicStore(ptr, val) (ptr)->store(val)
    #define b2AtomicExchange(ptr, val) (ptr)->exchange(val)
    #define b2AtomicCompareExchange(ptr, expected, desired) (ptr)->compare_exchange_weak(expected, desired)
    #define b2AtomicFetchAdd(ptr, val) (ptr)->fetch_add(val)
    #define b2AtomicFetchSub(ptr, val) (ptr)->fetch_sub(val)
#else
    // Standard atomic configuration for other platforms
    #define B2_HAS_ATOMIC 1
    #define b2Atomic(T) std::atomic<T>
    #define b2AtomicInit(ptr, val) std::atomic_init(ptr, val)
    #define b2AtomicLoad(ptr) std::atomic_load(ptr)
    #define b2AtomicStore(ptr, val) std::atomic_store(ptr, val)
    #define b2AtomicExchange(ptr, val) std::atomic_exchange(ptr, val)
    #define b2AtomicCompareExchange(ptr, expected, desired) std::atomic_compare_exchange_weak(ptr, expected, desired)
    #define b2AtomicFetchAdd(ptr, val) std::atomic_fetch_add(ptr, val)
    #define b2AtomicFetchSub(ptr, val) std::atomic_fetch_sub(ptr, val)
#endif
