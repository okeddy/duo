#include "Thread.h"

#include <boost/weak_ptr.hpp>

#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace duo {
    namespace CurrentThread {
        __thread const char* t_threadName = "unknown";
    }

    namespace {
        __thread pid_t t_cachedTid = 0;

        pid_t gettid() {
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }
    }

    void afterFork() {
        t_cachedTid = gettid();
        duo::CurrentThread::t_threadName = "main";
    }

    class ThreadNameInitializer
    {
    public:
        ThreadNameInitializer() {
            duo::CurrentThread::t_threadName = "main";
            pthread_atfork(NULL, NULL, &afterFork);
        }
    };

    ThreadNameInitializer init;

    struct ThreadDate {
        typedef duo::Thread::ThreadFunc ThreadFunc;
        ThreadFunc func_;
        std::string name_;
        boost::weak_ptr<pid_t> wkTid_;

        ThreadDate(const ThreadFunc& func,
                   const std::string& name,
                   const boost::shared_ptr<pid_t>& tid)
            : func_(func),
              name_(name),
              wkTid_(tid)
        { }

        void runTnThread() {
            pid_t tid = duo::CurrentThread::tid();
            boost::shared_ptr<pid_t> ptid = wkTid_.lock();

            if (ptid) {
                *ptid = tid;
                ptid.reset();
            }

            duo::CurrentThread::t_threadName = name_.empty() ? "duoThread" : name_.c_str();
            ::prctl(PR_SET_NAME, duo::CurrentThread::t_threadName);
            func_();
            duo::CurrentThread::t_threadName = "finished";
        }
    };
    
    void* startThread(void* obj) {
        ThreadDate* data = static_cast<ThreadDate*>(obj);
        data->runTnThread();
        delete data;
        return NULL;
    }
}

using namespace duo;

pid_t CurrentThread::tid() {
    if (t_cachedTid == 0) {
        t_cachedTid = gettid();
    }
    return t_cachedTid;
}

const char* CurrentThread::name() {
    return t_threadName;
}

bool CurrentThread::isMainThread() {
    return tid() == ::gettid();
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const std::string& name)
    : started_(false),
      joined_(false),
      pthreadId_(0),
      tid_(new pid_t(0)),
      func_(func),
      name_(name) {
    numCreated_.increment();
}

Thread::~Thread() {
    if (started_ && !joined_) {
        pthread_detach(pthreadId_);
    }
}

void Thread::start() {
    assert(!started_);
    started_ = true;

    ThreadDate* data = new ThreadDate(func_, name_, tid_);
    if (pthread_create(&pthreadId_, NULL, &startThread, data)) {
        started_ = false;
        delete data;
        abort();
    }
}

void Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    pthread_join(pthreadId_, NULL);
}
