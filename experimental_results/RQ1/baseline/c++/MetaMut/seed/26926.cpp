

#include <cstdio>

int main()
{    
    if (int* x=NULL) // <<< declare 'x'
    {}
    else if(true)
    {
        std::printf("%d\n", *x); // <<< You may access 'x' inside this scope
    }
    return 0;
}
