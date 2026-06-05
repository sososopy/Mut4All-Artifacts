
template<typename T> struct X { X(T) {} };
X(bool) -> X<int>;

namespace
{
    template<typename T> struct Y { Y(T) {} };
    Y(bool) -> Y<int>;
}

void test()
{
    [[maybe_unused]] X<int> x = X{false};
    [[maybe_unused]] Y<int> y = Y{false};
}
