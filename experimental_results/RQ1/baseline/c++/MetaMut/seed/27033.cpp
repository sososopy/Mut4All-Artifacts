
#include <iostream>
#include <exception>

class Base
{
public:
    virtual ~Base() {}
};

class C1 : public virtual Base
{
};

class C2 : public virtual Base
{
public:
    virtual void foo() = 0;
};

class D : public C1, public C2
{
public:
    virtual void foo() 
    {
        throw std::exception();
    }
};

int main()
{
    C2 * c2 = new D();

    try
    {
        c2->foo();
    }
    catch (...)
    {
        std::cout << "Caught some exception" << std::endl;
    }

    return 0;
}

