// Box2D Windows-specific configuration
#pragma once

#ifdef _MSC_VER
    #include <atomic>
    #include <cstdint>

    // Use standard C++ atomics for Box2D
    #define b2Atomic(T) std::atomic<T>
    #define b2AtomicInit(ptr, val) std::atomic_init(ptr, val)
    #define b2AtomicLoad(ptr) (ptr)->load(std::memory_order_acquire)
    #define b2AtomicStore(ptr, val) (ptr)->store(val, std::memory_order_release)
    #define b2AtomicExchange(ptr, val) (ptr)->exchange(val, std::memory_order_acq_rel)
    #define b2AtomicCompareExchange(ptr, expected, desired) \
        (ptr)->compare_exchange_weak(expected, desired, \
            std::memory_order_acq_rel, std::memory_order_acquire)
    #define b2AtomicFetchAdd(ptr, val) (ptr)->fetch_add(val, std::memory_order_acq_rel)
    #define b2AtomicFetchSub(ptr, val) (ptr)->fetch_sub(val, std::memory_order_acq_rel)

    // Ensure proper alignment
    #define B2_FORCE_INLINE __forceinline
    #define B2_ALIGN16 __declspec(align(16))
#endif
