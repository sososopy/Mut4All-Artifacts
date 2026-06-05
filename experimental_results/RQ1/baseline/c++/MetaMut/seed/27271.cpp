#include <stdbool.h>
#include <stdio.h>

bool f(int n)
{
    typedef int A[n];
    ++n;
    A a;
    int b[n];
    n -= 2;
    typedef int C[n];
    C c;

    return (sizeof(a) < sizeof(b)) && (sizeof(a) > sizeof(c));
}

int main()
{
        printf("%d", (int)f(10));
}

