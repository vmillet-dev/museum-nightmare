// Box2D MSVC-specific settings
#pragma once

#ifdef _MSC_VER
    #include <atomic>

    // Ensure we're using C++17
    #if _MSVC_LANG < 201703L
        #error "Box2D requires C++17 or later for proper atomic support"
    #endif

    // Use standard C++17 atomics
    #define b2Atomic(T) std::atomic<T>
    #define b2AtomicInit(ptr, val) (ptr)->store(val, std::memory_order_release)
    #define b2AtomicLoad(ptr) (ptr)->load(std::memory_order_acquire)
    #define b2AtomicStore(ptr, val) (ptr)->store(val, std::memory_order_release)
    #define b2AtomicExchange(ptr, val) (ptr)->exchange(val, std::memory_order_acq_rel)
    #define b2AtomicCompareExchange(ptr, expected, desired) \
        (ptr)->compare_exchange_weak(expected, desired, \
            std::memory_order_acq_rel, std::memory_order_acquire)
    #define b2AtomicFetchAdd(ptr, val) (ptr)->fetch_add(val, std::memory_order_acq_rel)
    #define b2AtomicFetchSub(ptr, val) (ptr)->fetch_sub(val, std::memory_order_acq_rel)

    // Additional MSVC-specific settings
    #define B2_FORCE_INLINE __forceinline
    #define B2_ALIGN16 __declspec(align(16))
    #define B2_HAS_ATOMIC 1
#endif
