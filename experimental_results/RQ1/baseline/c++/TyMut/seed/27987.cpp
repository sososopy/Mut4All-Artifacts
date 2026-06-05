

#include <stdio.h>

struct Test
{
    operator int(){ return 0; } 
    operator double(){ return 0; }
};

int main()
{
    printf("Hello World");
    
    int test[2] = {};
    int test2 = test[Test()];

    return 0;
}
