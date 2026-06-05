

#include <type_traits>

int main()
{
        auto const g([](auto && _var) {
                static_assert(std::is_same<int &&,decltype(_var)>::value,"");
        });

        g(0);
}

