#include ".\thread.h"
#include <malloc.h>
#include "ThreadPool.h"

 # pragma warning(disable: 4731)

Thread::Thread(void)
{
  _stack_length = NULL;
  _stack_start = NULL;
  _stack_pointer = NULL;
  _thread_begin_function = NULL;
}

Thread::Thread(uint32 size, void (*begin_function)(void))
{
  _stack_length = size;
  _stack_start = (uint8*)malloc(size);
  _stack_pointer = _stack_start + _stack_length; //TODO: add a function that will kill the thread to the stack
  _thread_begin_function = begin_function;
  initStack();
}


Thread::~Thread(void)
{
  if( _stack_start != NULL && _stack_length != NULL)
  {
    free(_stack_start);
  }
}

void Thread::endThread()
{
  _asm
  {
    mov ECX, [EBP+4h]
    mov this, ECX
  }
  ThreadPool::instance()->removeThread(this);
}

//platform specific ASM, it depends on the compiler pushing the same onto the stack at the beginning of each function,

void Thread::pauseThread()
{
  //std::cout << "pauseThread:" << Name.c_str() <<std::endl;
  _asm
  {
    mov EAX, this
    mov [EAX], ESP
  }//Save the extended stack pointer.
  ThreadPool::instance()->switchThread();
}

void Thread::resumeThread()
{
  //TODO: add a destroy function to the stack.
  _asm
  { 
    mov EAX, this//EAX points to _stack_pointer
    sub EBP, ESP // The Difference bewteen the framepointer and stack pointer
    mov ESP, [EAX]
    add EBP, ESP
    mov this, EAX //WHOAH!
  }
}

void Thread::initStack()
{
  _asm
  {
    
    mov EAX, this//EAX points to _stack_pointer
    // Save the ESP, EBP
    sub EBP, ESP
    mov ESI, ESP
    
    //set the stack to the beginning of allocated memory
    mov ESP, [EAX]

    //Push the return function onto the stack
    push EAX //pushing this
    push Thread::endThread
    mov EBX, [EAX+_thread_begin_function]
    push EBX;
    push EBP;
      
    //now set the ESP to allow for the pops
    sub ESP, EBP

    // Save the new ESP
    mov [EAX], ESP
    // Reset the ESP
    mov ESP, ESI
    add EBP, ESP
  }
}
