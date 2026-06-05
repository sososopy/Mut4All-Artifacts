
#include <cassert>

struct Positive { constexpr Positive(int x) {assert(x >= 0);} };

int foo(Positive) {return 1;}

int main()
{
    return foo(Positive{1});
}

GCC 10.1 and 10.2 will generate code to check if the assertion is true even though it is statically checkable. This occurs for optimization levels -O0, -Os, and -O2, but the assertion is elided for optimization levels -Og, -O1, and -O3.

The assertion is always elided in GCC 9.4 and below as well as GCC 10.3 and above.
Please see https://godbolt.org/z/3scPr1P7W for a comparison of x86-64 assembly with different GCC versions.
---------------------------------------------------------

