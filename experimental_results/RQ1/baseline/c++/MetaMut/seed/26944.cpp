

#include <memory>
#include <iostream>

struct X
{
    ~X () { std::cout << "Bye" << std::endl; }
};

struct Y
{
    explicit Y (std::unique_ptr<X> x)
    : m_x (std::move (x))
    { }

    std::unique_ptr<X> m_x;
};

int main ()
{
    using F = Y (*) (std::unique_ptr<X>);
    auto p = std::unique_ptr<X> {new X};

#if 1
    F f =
#else
    auto f =
#endif
    [] (std::unique_ptr<X> x)
    {
        return Y (std::move (x));
    };
    f (std::move (p));
}
