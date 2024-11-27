#pragma once

// Box2D configuration for cross-platform atomic support
#ifdef _MSC_VER
    #include <atomic>

    // Use standard C++17 atomics for MSVC
    #define b2Atomic(T) std::atomic<T>
    #define b2AtomicInit(ptr, val) std::atomic_init(ptr, val)
    #define b2AtomicLoad(ptr) std::atomic_load(ptr)
    #define b2AtomicStore(ptr, val) std::atomic_store(ptr, val)
    #define b2AtomicExchange(ptr, val) std::atomic_exchange(ptr, val)
    #define b2AtomicCompareExchangeWeak(ptr, expected, desired) \
        std::atomic_compare_exchange_weak(ptr, expected, desired)
    #define b2AtomicFetchAdd(ptr, val) std::atomic_fetch_add(ptr, val)
    #define b2AtomicFetchSub(ptr, val) std::atomic_fetch_sub(ptr, val)
#else
    // Use Box2D's default atomic implementation for other platforms
    #include <box2d/b2_settings.h>
#endif

// Ensure proper memory ordering
#define B2_MEMORY_ORDER_RELAXED std::memory_order_relaxed
#define B2_MEMORY_ORDER_ACQUIRE std::memory_order_acquire
#define B2_MEMORY_ORDER_RELEASE std::memory_order_release
#define B2_MEMORY_ORDER_ACQ_REL std::memory_order_acq_rel
