#pragma once

// Box2D configuration for cross-platform atomic support
#ifdef _MSC_VER
    #include <atomic>
    #include <cstdint>

    // Use C++17 std::atomic for MSVC
    #define b2Atomic(T) std::atomic<T>
    #define b2AtomicInt32 std::atomic<int32_t>
    #define b2AtomicBool std::atomic<bool>

    // Atomic operations with explicit memory ordering
    #define b2AtomicInit(ptr, val) std::atomic_init(ptr, val)
    #define b2AtomicLoad(ptr) std::atomic_load_explicit(ptr, std::memory_order_acquire)
    #define b2AtomicStore(ptr, val) std::atomic_store_explicit(ptr, val, std::memory_order_release)
    #define b2AtomicExchange(ptr, val) std::atomic_exchange_explicit(ptr, val, std::memory_order_acq_rel)
    #define b2AtomicCompareExchangeWeak(ptr, expected, desired) \
        std::atomic_compare_exchange_weak_explicit(ptr, expected, desired, std::memory_order_acq_rel, std::memory_order_acquire)
    #define b2AtomicFetchAdd(ptr, val) std::atomic_fetch_add_explicit(ptr, val, std::memory_order_acq_rel)
    #define b2AtomicFetchSub(ptr, val) std::atomic_fetch_sub_explicit(ptr, val, std::memory_order_acq_rel)
#else
    // Use Box2D's default atomic implementation for other platforms
    #include <box2d/b2_settings.h>
#endif

// Ensure proper memory ordering
#define B2_MEMORY_ORDER_RELAXED std::memory_order_relaxed
#define B2_MEMORY_ORDER_ACQUIRE std::memory_order_acquire
#define B2_MEMORY_ORDER_RELEASE std::memory_order_release
#define B2_MEMORY_ORDER_ACQ_REL std::memory_order_acq_rel
