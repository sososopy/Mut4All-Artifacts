
#ifndef CLASS
#define CLASS
static int a;
class Class
{
public:
    Class();
    ~Class();
private:
    int b;
    int c;
};
#endif


#include <stdio.h>
Class::Class() : b(0), c(0)
{
    a++;
    printf("%d\n", a);
};

Class::~Class()
{
    printf("%d\n", a);
    a--;
}


int main()
{
    Class a, b, c, d;
    return 0;
}

