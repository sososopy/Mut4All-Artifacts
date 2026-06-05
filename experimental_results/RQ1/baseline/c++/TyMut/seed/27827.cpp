
#include <stdio.h>
#include <iostream>
using namespace std;

class A
{
    public:
        A(){}
        virtual ~A(){};
};

class B : public A
{
    public:
        B(){}
        virtual ~B(){}
};

int main()
{
    A *pa, *pa2;
    B *pb, *pb2;
    pa = new A; 
    pb = new B; 

    delete pb;
    pa2 = dynamic_cast<A*>(pb);
    pb2 = dynamic_cast<B*>(pb);
    pb2 = dynamic_cast<B*>(pa2);
    printf("pb2 = %p\n", pb2);

    pb2 = dynamic_cast<B*>(pa);
    printf("pb2 = %p\n", pb2);

    delete pa;
    delete pb;

    return 0; 
}

