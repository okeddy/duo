#include "thread/Mutex.h"

class Request
{
 public:
  void process() // __attribute__ ((noinline))
  {
    duo::MutexLockGuard lock(mutex_);
    print();
  }

  void print() const // __attribute__ ((noinline))
  {
    duo::MutexLockGuard lock(mutex_);
  }

 private:
  mutable duo::MutexLock mutex_;
};

int main()
{
  Request req;
  req.process();
}
