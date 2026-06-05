
#include "a.h"

int A::f()
{
        return n+1;
}

explicit A::A(int _n)   // <=== error at this line
{
        n = _n;
}



