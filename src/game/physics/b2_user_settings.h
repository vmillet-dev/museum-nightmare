#pragma once

#include <atomic>

// Use standard C++17 atomics
#define B2_HAS_ATOMIC 1
#define b2Atomic(T) std::atomic<T>

// Use simple atomic operations without explicit memory ordering
#define b2AtomicInit(ptr, val) std::atomic_init(ptr, val)
#define b2AtomicLoad(ptr) std::atomic_load(ptr)
#define b2AtomicStore(ptr, val) std::atomic_store(ptr, val)
#define b2AtomicExchange(ptr, val) std::atomic_exchange(ptr, val)
#define b2AtomicCompareExchange(ptr, expected, desired) std::atomic_compare_exchange_weak(ptr, expected, desired)
#define b2AtomicFetchAdd(ptr, val) std::atomic_fetch_add(ptr, val)
#define b2AtomicFetchSub(ptr, val) std::atomic_fetch_sub(ptr, val)
