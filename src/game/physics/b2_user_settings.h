// Box2D user settings for Windows compatibility
#pragma once

#ifdef _MSC_VER
    #include <atomic>
    #define b2Atomic(T) std::atomic<T>
    #define b2AtomicInit(ptr, val) std::atomic_init(ptr, val)
    #define b2AtomicLoad(ptr) std::atomic_load(ptr)
    #define b2AtomicStore(ptr, val) std::atomic_store(ptr, val)
    #define b2AtomicExchange(ptr, val) std::atomic_exchange(ptr, val)
    #define b2AtomicCompareExchange(ptr, expected, desired) std::atomic_compare_exchange_weak(ptr, expected, desired)
    #define b2AtomicFetchAdd(ptr, val) std::atomic_fetch_add(ptr, val)
    #define b2AtomicFetchSub(ptr, val) std::atomic_fetch_sub(ptr, val)
#endif
