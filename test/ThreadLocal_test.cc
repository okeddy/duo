/*
* Copyright (c) 2021, kd.
* All rights reserved.
*
* 文件名称：ThreadLocal_test.cc
* 摘 要：
*
* 当前版本：1.0
* 作 者：kd
* 完成日期：2021年2月19日
*/

#include "../base/ThreadLocal.h"
#include "../base/Thread.h"

#include <boost/noncopyable.hpp>
#include <stdio.h>

class Test : boost::noncopyable
{
 public:
  Test()
  {
    printf("tid=%d, 构造 %p\n", duo::CurrentThread::tid(), this);
  }

  ~Test()
  {
    printf("tid=%d, 折构 %p %s\n", duo::CurrentThread::tid(), this, name_.c_str());
  }

  const std::string& name() const { return name_; }
  void setName(const std::string& n) { name_ = n; }

 private:
  std::string name_;
};

duo::ThreadLocal<Test> testObj1;
duo::ThreadLocal<Test> testObj2;

void print()
{
  printf("tid=%d, obj1 %p name=%s\n",
         duo::CurrentThread::tid(),
	 &testObj1.value(),
         testObj1.value().name().c_str());
  printf("tid=%d, obj2 %p name=%s\n",
         duo::CurrentThread::tid(),
	 &testObj2.value(),
         testObj2.value().name().c_str());
}

void threadFunc()
{
  print();
  testObj1.value().setName("子线程obj 1");
  testObj2.value().setName("子线程obj 2");
  print();
}

int main()
{
  testObj1.value().setName("主线程obj 1");
  print();
  duo::Thread t1(threadFunc);
  t1.start();
  t1.join();
  testObj2.value().setName("主线程obj 2");
  print();

  pthread_exit(0);
}
