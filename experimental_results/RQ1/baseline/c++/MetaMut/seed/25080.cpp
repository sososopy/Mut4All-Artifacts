
#include <iostream>
#include <type_traits>

struct Foo
{
    template <typename T> void foo(T&& x) { x.hello();}
};

struct Caller
{    
    template <typename T>
    auto call(T&& x, int) -> decltype(
        std::enable_if_t<
            std::is_same<
                decltype(&T::template foo<decltype(*this)>),
                void (T::*)(decltype(*this))
            >::value
        >())
    {
        //x.foo(*this);
    }
    
    template <typename T>
    void call(T&&, char){ std::cout << "hello" << std::endl;}
};

int main()
{
  Caller c;
  c.call(Foo(), 0);
}
