

#if 0
#  include <memory>
#else
inline void* operator new(unsigned long, void* __p) { return __p; }
#endif

#include <stdio.h>

typedef int* T;

static T storage;
static T* p = &storage;

// '__attribute__((__always_inline__))' seems to be the trigger
static inline __attribute__((__always_inline__)) void append(T value)
{
    new (p) T(value);
}

int main() {
    int a;
    append(&a);

    if (!*p)
        __builtin_trap();
    fprintf(stderr, "&a=%p\n", *p);
}
