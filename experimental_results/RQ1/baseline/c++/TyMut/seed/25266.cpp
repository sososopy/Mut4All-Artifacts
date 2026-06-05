
#include <algorithm>
#include <iterator>
#include <iostream>

template <typename T>
struct Base
{
        using value_type = T;

        void func(T v)
        {
                std::cout << v << a << '\n';
        }

        T a{5};
};

template <typename T>
struct Derived : T
{
        using typename T::value_type;

        // here's the bug, maybe?
        void do_something()
        {
                // Call member function of the base class. Everything's fine.
                T::func(arr[0]);

                // Everything is fine here also:
                auto lambda = [this](auto a) { T::func(a); };
                lambda(arr[1]);

                // Everything's fine here, too. Non-generic lambda.
                std::for_each(std::begin(arr), std::end(arr), [this](int a) { T::func(a); });

                // Here's the error: g++ thinks, that T::func(a) is a call without object, even
                // though "this" is explicitly captured. Same as above, but generic lambda.
                std::for_each(std::begin(arr), std::end(arr), [this](auto a) { T::func(a); });
        }

        value_type arr[16]{};
};

int main()
{
        Derived<Base<int>> o;
        o.do_something();
        return 0;
}
