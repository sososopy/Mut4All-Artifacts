
#include <immintrin.h>

__m512 f(__m512i a)
{
    return _mm512_cvtepi32_ps(a);
}

