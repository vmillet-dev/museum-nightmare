// Box2D user settings for Windows compatibility
#pragma once

#ifdef _MSC_VER
    #include <atomic>
    #include <cstdint>

    // Ensure proper alignment for atomic types
    #define b2_atomic_alignment alignas(sizeof(void*))

    // Use std::atomic with proper alignment
    template<typename T>
    using b2AtomicType = std::atomic<T>;

    #define b2Atomic(T) b2AtomicType<T>
    #define b2AtomicInit(ptr, val) std::atomic_init(ptr, val)
    #define b2AtomicLoad(ptr) (ptr)->load(std::memory_order_acquire)
    #define b2AtomicStore(ptr, val) (ptr)->store(val, std::memory_order_release)
    #define b2AtomicExchange(ptr, val) (ptr)->exchange(val)
    #define b2AtomicCompareExchange(ptr, expected, desired) (ptr)->compare_exchange_weak(expected, desired)
    #define b2AtomicFetchAdd(ptr, val) (ptr)->fetch_add(val)
    #define b2AtomicFetchSub(ptr, val) (ptr)->fetch_sub(val)

    // Disable experimental C11 atomics warning
    #pragma warning(disable: 4841)
#endif
