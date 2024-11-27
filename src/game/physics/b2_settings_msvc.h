// Box2D MSVC-specific settings
#pragma once

#ifdef _MSC_VER
    #include <atomic>
    #include <cstdint>

    // Use C++17 standard atomics
    namespace b2 {
        template<typename T>
        using atomic = std::atomic<T>;
    }

    // Define atomic operations using C++17 std::atomic
    #define b2Atomic(T) b2::atomic<T>
    #define b2AtomicInit(ptr, val) new (ptr) b2::atomic<decltype(val)>(val)
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

    // Disable specific MSVC warnings
    #pragma warning(disable: 4324)  // structure was padded due to alignment specifier
#endif
