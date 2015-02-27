#include <sys/stat.h>

extern caddr_t _get_stack_pointer(void);

caddr_t _sbrk(int incr) {
  extern char _end;
  static char* heap_end;
  char* prev_heap_end;

  if (heap_end == 0) {
    heap_end = &_end;
  }

  prev_heap_end = heap_end;

  if ((heap_end + incr) > _get_stack_pointer()) {
    while (1) {
      // TRAP!!!
    }
  }

  heap_end += incr;
  return (caddr_t)prev_heap_end;
}
