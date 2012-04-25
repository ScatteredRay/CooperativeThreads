#pragma once
#include "primitives.h"

#include <iostream>

#define CheckNow if(_stack_start+_stack_length >= _stack_pointer && _stack_pointer >= _stack_start) std::cout << Name.c_str() << ": In Bounds" << std::endl; \
else  std::cout << Name.c_str() << ": Out of Bounds" << std::endl;


class Thread
{
public:
  uint8 *_stack_pointer; // Must be very first item of class, or will screw up switching
  uint8 *_stack_start;
  uint32 _stack_length;
  std::string Name;
private:
  void (*_thread_begin_function)(void);
public:
  Thread(void);
  Thread(uint32 size, void (*begin_function)(void));
  ~Thread(void);
  void pauseThread();
  void resumeThread();
private:
  void initStack();
  void endThread();
};
