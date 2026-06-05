
#include <xmmintrin.h>
#include <stdint.h>

#define LOAD_SI128(ptr) \
        ( ((uintptr_t)(ptr) & 15) == 0 ) ? _mm_load_si128((__m128i*)(ptr)) : _mm_loadu_si128((__m128i*)(ptr))

extern char x[16];
__m128i y;

void
test ()
{
  y = LOAD_SI128 (&x);
}

