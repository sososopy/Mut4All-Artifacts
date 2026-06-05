
#include <tuple>

template <typename T>
using B = std::tuple<T*>;

using A = std::tuple<B<A>>;

int main()
{
    B b;
}
