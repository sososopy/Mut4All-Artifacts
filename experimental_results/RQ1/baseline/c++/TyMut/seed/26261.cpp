
#include <cstdio>

extern "C"{
void _Z3foov()
{
    printf("Bug\n");
}
}

void foo()
{
    printf("Correct\n");
}

int main()
{
    foo();
}
