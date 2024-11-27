// Box2D user settings for Windows compatibility
#pragma once

#ifdef _MSC_VER
    #include <atomic>
    #include <cstdint>
    #include <type_traits>

    // Ensure proper alignment for atomic types
    #define b2_atomic_alignment alignas(sizeof(void*))

    // Use std::atomic with proper memory ordering
    template<typename T>
    struct b2AtomicWrapper {
        std::atomic<T> value;

        b2AtomicWrapper() : value{} {}
        b2AtomicWrapper(T v) : value{v} {}

        operator T() const { return value.load(std::memory_order_acquire); }
        T operator=(T v) { value.store(v, std::memory_order_release); return v; }
    };

    #define b2Atomic(T) b2AtomicWrapper<T>
    #define b2AtomicInit(ptr, val) new (ptr) b2AtomicWrapper<decltype(val)>(val)
    #define b2AtomicLoad(ptr) static_cast<decltype(ptr->value.load())>((ptr)->value.load(std::memory_order_acquire))
    #define b2AtomicStore(ptr, val) (ptr)->value.store(val, std::memory_order_release)
    #define b2AtomicExchange(ptr, val) (ptr)->value.exchange(val, std::memory_order_acq_rel)
    #define b2AtomicCompareExchange(ptr, expected, desired) (ptr)->value.compare_exchange_weak(expected, desired, std::memory_order_acq_rel)
    #define b2AtomicFetchAdd(ptr, val) (ptr)->value.fetch_add(val, std::memory_order_acq_rel)
    #define b2AtomicFetchSub(ptr, val) (ptr)->value.fetch_sub(val, std::memory_order_acq_rel)

    // Disable experimental C11 atomics warning
    #pragma warning(disable: 4841)
#endif
