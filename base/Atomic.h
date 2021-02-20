/*
* Copyright (c) 2021, kd.
* All rights reserved.
*
* 文件名称：Atomic.h
* 摘 要：
*
* 当前版本：1.0
* 作 者：kd
* 完成日期：2021年2月19日
*/

#ifndef DUO_BASE_ATOMIC_H
#define DUO_BASE_ATOMIC_H

#include <boost/noncopyable.hpp>
#include <stdint.h>

namespace duo {
    namespace detail {
        template<typename T>
        class AtomicIntegerT : boost::noncopyable {
        private:
            volatile T value_;
        public:
            AtomicIntegerT() : value_(0) { }

            T get() const {
                return __sync_val_compare_and_swap(const_cast<volatile T*>(&value_), 0, 0);
            }

            T getAndAdd(T x) {
                return __sync_fetch_and_add(&value_, x);
            }

            T addAndGet(T x) {
                return getAndAdd(x) + x;
            }

            T incrementAndGet() {
                return addAndGet(1);
            }

            void add(T x) {
                getAndAdd(x);
            }

            void increment() {
                incrementAndGet();
            }

            void decrement() {
                getAndAdd(-1);
            }

            T getAndSet(T newValue) {
                return __sync_lock_test_and_set(&value_, newValue);
            }
        };
    }

    typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
    typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
}

#endif  // DUO_BASE_ATOMIC_H
