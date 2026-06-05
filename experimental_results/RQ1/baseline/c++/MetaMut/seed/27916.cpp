

#include <iostream>
#include <xmmintrin.h>

void foo(const float num, const float denom)
{
    // typedef __v4sf Vec4;
    typedef __m128 Vec4;
    const Vec4 num4 = {
        num,
        num,
        num,
        num,
    };
    const Vec4 denom4 = {
        denom,
        denom,
        denom,
        denom,
    };
    float res_arr[] = {0, 0, 0, 0};

    Vec4 *res = (Vec4*)res_arr;
    *res = num4 / denom4;
    std::cout << res_arr[0] << std::endl;
    std::cout << res_arr[1] << std::endl;
    std::cout << res_arr[2] << std::endl;
    std::cout << res_arr[3] << std::endl;
}

//b.h:
#ifndef B_H
#define B_H

void foo(const float num, const float denom);

#endif

//a.cpp:
#include "b.h"

int main (void)
{
    const float denominator = 1.0f;
    const float numerator = 12.0f;
    foo(numerator, denominator);
    return 0;
}

