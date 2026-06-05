

#include <iostream>

class A
{
public:
    static void hello() {
        std::cerr << "A::hello()" << std::endl;
    }
};

class B
{
public:
    static void hello() {
        std::cerr << "B::hello()" << std::endl;
    }
};

template<typename type>
class Base
{
public:
    typedef type type_t;
    static void hello() {
        typedef int type_t; // <<< shadowing type_t
        type_t x = type_t();
    }
};

template<typename type>
class Derived_typedef : public Base<A>
{
public:
    typedef type type_t; // <<< shadowing Base<A>::type_t
    static void hello() { type_t::hello(); }
};

template<typename type_t>
class Derived_template_param : public Base<A> // <<< shadowing template parm type_t
{
public:
    static void hello() { type_t::hello(); }
};

int main()
{
    Derived_typedef<B>::hello();
    Derived_template_param<B>::hello();
    return 0;
}
