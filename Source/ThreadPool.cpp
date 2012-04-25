#include ".\threadpool.h"

ThreadPool::ThreadPool(void)
{
}

ThreadPool::~ThreadPool(void)
{
}

ThreadPool* ThreadPool::instance()
{
  static ThreadPool pool;
  return &pool;
}

void ThreadPool::switchThread()
{
  //TODO: the deletion scheme needs a rework
    if(!_delete_queue.empty() && _current_thread != _delete_queue.front())
    {
      delete _delete_queue.front();
      _delete_queue.pop();
    }
    Thread *next_thread = _threads.front();
    _threads.pop();
    _threads.push(next_thread);
    _current_thread = next_thread;
    next_thread->resumeThread();
}


// Size in bytes
Thread* ThreadPool::newThread(int size, void (*begin_function)(void))
{
  Thread* thread = new Thread(size, begin_function);
  _threads.push(thread);
  return thread;
}

void ThreadPool::addThread(Thread *thread)
{
  _threads.push(thread);
}

Thread* ThreadPool::getCurrentThread()
{
  return _current_thread;
}

void ThreadPool::beginThread(Thread *thread)
{

}

void ThreadPool::removeThread(Thread *thread)
{
  //TODO: optimization.
  Thread *tmpThread;
  while(true) //TODO: Prevent infinite loops
  {
    tmpThread = _threads.front();
    _threads.pop();
    if(tmpThread != thread) _threads.push(tmpThread);
    else
    {
      break;
    }
  }
  _delete_queue.push(thread); //unconditional delete, but can't be done here, we are still likely in the thread.
  ThreadPool::instance()->switchThread();
}