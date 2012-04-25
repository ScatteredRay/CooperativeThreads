#pragma once

#include "Thread.h"
#include <queue>

class ThreadPool
{
public:
  ThreadPool(void);
  ~ThreadPool(void);
  static ThreadPool* instance();
  void switchThread();
  Thread* newThread(void (*begin_function)(void)){return newThread(1048576, begin_function);}
  Thread* newThread(int size, void (*begin_function)(void));
  void addThread(Thread *thread);
  void removeThread(Thread *thread);
  Thread* getCurrentThread();
  void beginThread(Thread* thread);
private:
  std::queue<Thread*> _threads;
  std::queue<Thread*> _delete_queue;
  Thread* _current_thread;
};
