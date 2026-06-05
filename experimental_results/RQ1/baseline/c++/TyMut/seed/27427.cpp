
#include <stdio.h>
#include <stdint.h>
class A
{ 
    public:
        virtual void print()
        {
            printf("A\n");
        }
};
class B
{
    public:
        virtual void print()
        {
            printf("B\n");
        }
};
int main()
{
    A a;
    B b;
    A *pa=&a;
    pa->print();
    pa=(A *)&b;
    pa->print();
    return 0;
}
