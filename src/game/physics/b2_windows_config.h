// Box2D Windows-specific configuration
#pragma once

#ifdef _MSC_VER
    #include <atomic>
    #include <cstdint>

    // Ensure proper alignment and disable warnings
    #pragma warning(disable: 4324)  // structure was padded due to alignment specifier
    #pragma warning(disable: 4841)  // non-standard extension used

    // Use standard C++ atomics
    #define B2_HAS_ATOMIC 1
    #define B2_FORCE_INLINE __forceinline

    // Define atomic operations using std::atomic
    template<typename T>
    struct alignas(sizeof(T)) b2Atomic_t {
        std::atomic<T> value;

        b2Atomic_t() : value{} {}
        explicit b2Atomic_t(T v) : value{v} {}

        operator T() const { return value.load(std::memory_order_acquire); }
        T operator=(T v) { value.store(v, std::memory_order_release); return v; }
    };

    #define b2Atomic(T) b2Atomic_t<T>
    #define b2AtomicInit(ptr, val) new (ptr) b2Atomic_t<decltype(val)>(val)
    #define b2AtomicLoad(ptr) (ptr)->value.load(std::memory_order_acquire)
    #define b2AtomicStore(ptr, val) (ptr)->value.store(val, std::memory_order_release)
    #define b2AtomicExchange(ptr, val) (ptr)->value.exchange(val, std::memory_order_acq_rel)
    #define b2AtomicCompareExchange(ptr, expected, desired) (ptr)->value.compare_exchange_weak(expected, desired, std::memory_order_acq_rel)
    #define b2AtomicFetchAdd(ptr, val) (ptr)->value.fetch_add(val, std::memory_order_acq_rel)
    #define b2AtomicFetchSub(ptr, val) (ptr)->value.fetch_sub(val, std::memory_order_acq_rel)
#endif
