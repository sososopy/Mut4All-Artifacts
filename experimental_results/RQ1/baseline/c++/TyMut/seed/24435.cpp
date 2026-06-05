
#include <iostream>
#include <utility>
#include <string>

class Base
{
public:
        Base() { std::cout << this << " Base ctor\n" ; }
        ~Base() { std::cout << this << " Base dtor\n" ; }
        Base(Base const&) { std::cout << this << " Base copy\n" ; }
        Base& operator=(Base const&) { std::cout << this << " Base copy assign\n" ; return *this; }
        Base(Base&&) { std::cout << this << " Base move\n" ; }
        Base& operator=(Base&&) { std::cout << this << " Base move assign\n" ; return *this; }
};

class Derived : public Base
{
public:
        Derived(): Base() { std::cout << this << " Derived ctor\n" ; }
        ~Derived() { std::cout << this << " Derived dtor\n" ; }
        Derived(Derived const& o): Base(o) { std::cout << this << " Derived copy\n" ; }
        Derived(Derived&& o): Base(std::move(o)) { std::cout << this << " Derived move\n" ; }
        Derived& operator=(Derived const& o) { std::cout << this << " Derived copy assign\n" ; return (*this) ; }
        Derived& operator=(Derived&& o) { std::cout << this << " Derived move assign\n" ; Base::operator=(std::move(o)) ; return (*this) ; }
};

Derived f(Derived d) { return (d) ; }

int main()
{
        Derived r ;
        Derived u ;
        u = f(r) ;
        return 0;
}
