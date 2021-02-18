#ifndef DUO_BASE_THREADLOCAL_H
#define DUO_BASE_THREADLOCAL_H

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace duo {
    template<typename T>
    class ThreadLocal : boost::noncopyable
    {
    public:
        ThreadLocal() {
            pthread_key_create(&pkey_, &ThreadLocal::destructor);
        }

        ~ThreadLocal() {
            pthread_key_delete(pkey_);
        }

        T& value() {
            T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));
            if (!perThreadValue) {
                T* newObj = new T();
                pthread_setspecific(pkey_, newObj);
                perThreadValue = newObj;
            }
            return *perThreadValue;
        }

    private:
        static void destructor(void *x) {
            T* obj = static_cast<T*>(x);
            delete obj;
        }

        pthread_key_t pkey_;
    };
}

#endif  // DUO_BASE_THREADLOCAL_H
