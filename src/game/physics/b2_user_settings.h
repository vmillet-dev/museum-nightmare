// Box2D user settings for Windows compatibility
#pragma once

#ifdef _MSC_VER
    #include <atomic>
    #include <cstdint>
    #include <type_traits>

    // Ensure proper alignment for atomic types
    #define b2_atomic_alignment alignas(sizeof(void*))

    // Custom atomic wrapper that doesn't rely on C11 atomics
    template<typename T>
    class b2AtomicWrapper {
    private:
        alignas(sizeof(T)) std::atomic<T> value;

    public:
        b2AtomicWrapper() noexcept : value{} {}
        explicit b2AtomicWrapper(T v) noexcept : value{v} {}

        T load() const noexcept { return value.load(std::memory_order_acquire); }
        void store(T desired) noexcept { value.store(desired, std::memory_order_release); }
        T exchange(T desired) noexcept { return value.exchange(desired, std::memory_order_acq_rel); }
        bool compare_exchange_weak(T& expected, T desired) noexcept {
            return value.compare_exchange_weak(expected, desired, std::memory_order_acq_rel);
        }
        T fetch_add(T arg) noexcept { return value.fetch_add(arg, std::memory_order_acq_rel); }
        T fetch_sub(T arg) noexcept { return value.fetch_sub(arg, std::memory_order_acq_rel); }
    };

    #define b2Atomic(T) b2AtomicWrapper<T>
    #define b2AtomicInit(ptr, val) new (ptr) b2AtomicWrapper<decltype(val)>(val)
    #define b2AtomicLoad(ptr) (ptr)->load()
    #define b2AtomicStore(ptr, val) (ptr)->store(val)
    #define b2AtomicExchange(ptr, val) (ptr)->exchange(val)
    #define b2AtomicCompareExchange(ptr, expected, desired) (ptr)->compare_exchange_weak(expected, desired)
    #define b2AtomicFetchAdd(ptr, val) (ptr)->fetch_add(val)
    #define b2AtomicFetchSub(ptr, val) (ptr)->fetch_sub(val)
#endif
