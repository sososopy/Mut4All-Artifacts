

#include <type_traits>

struct Base { };

template<typename T>
struct Constraint1
: std::enable_if<std::is_base_of<Base, T>::value>
{ };

template <typename T,
         typename Requires = typename Constraint1<T>::type>
void foo() { }

struct A { };

int main()
{
    foo<A>();
}

