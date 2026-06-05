
#include <type_traits>

using Array = int[];

template <typename ...Ts>
void bar1(Ts ...)
{
    auto && array = Array{ 1, 2, 3 };

    static_assert(std::is_same<int (&&)[3], decltype(array)>{}, ""); // this fails, deduces array as int (&&) []
}

template <typename T>
void bar2()
{
    auto && array = Array{ 1, 2, 3 };

    static_assert(std::is_same<int (&&)[3], decltype(array)>{}, "");  // this fails, deduces array as int (&&) []
}

void bar3()
{
    auto && array = Array{ 1, 2, 3 };

    static_assert(std::is_same<int (&&)[3], decltype(array)>{}, ""); // OK
}

int main()
{
    bar1<int>(1, 2, 3);
    bar2<int>();
    bar3();
}
