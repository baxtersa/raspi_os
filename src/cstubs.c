#include <sys/stat.h>

/* A helper function in assembler to help allocate memory */
extern caddr_t _get_stack_pointer(void);

/* Increase program data space. As malloc and related functions depend on this,
   it is useful to have a working implementation. The following suffices for a
   standalone system; it exploits the symbol _end automatically defined by the GNU linker. */
caddr_t _sbrk (int incr) {
    extern char _end;
    static char* heap_end = 0;
    char* prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_end;
    }

    prev_heap_end = heap_end;

    heap_end += incr;
    return (caddr_t) prev_heap_end;
}

/* Status of an open file. For consistency with other minimal implementations
   in these examples, all files are regarded as character special devices. The
   sys/stat.h header file required is distributed in the include subdirectory
   for this C library. */
int _fstat (int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

/* Query whether output stream is a terminal. For consistency with the other minimal
   implementations, which only support output to stdout, this minimal implementation
   is suggested. */
int _isatty (int file) {
    return 1;
}

/* There's currently no implementation of a file system because there's no
   file system! */
int _close (int file) {
    return -1;
}

/* Set position in a file. Minimal implementation */
int _lseek (int file, int ptr, int dir) {
    return 0;
}

/* Read from a file. Minimal implementation */
int _read (int file, char *ptr, int len) {
    return 0;
}

void outbyte (char b) {
    return;
}

int _write (int file, char *ptr, int len) {
    int todo;

    for (todo = 0; todo < len; todo++) {
        outbyte (*ptr++);
    }

    return len;
}
