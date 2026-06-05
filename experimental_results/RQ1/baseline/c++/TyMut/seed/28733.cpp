
//a.h

#pragma once

struct A
{
    static thread_local int x;
};

//a.cpp

#include "a.h"

thread_local int A::x = 0;

//main.cpp

#include "a.h"

int main()
{
    A::x = 1;
    return 0;
}

