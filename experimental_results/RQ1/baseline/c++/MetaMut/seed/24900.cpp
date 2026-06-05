
export module mod;
export import :part;

export void foo()
{
}


module;

#include "bar.hpp"

export module mod:part;

export using ::bar;

export void bar_non_gmf()
{
}


import mod;

int main()
{
    foo();
    bar();
    bar_non_gmf();
}

