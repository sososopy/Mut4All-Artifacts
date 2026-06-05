
#include <initializer_list>

template <template<typename...> class C, typename... T> void g(const C<T...>& a)
{
}

int main()
{
    g<std::initializer_list, int>({1, 2, 3});
}

