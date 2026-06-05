
#include <type_traits>

template<typename T>
    //requires std::is_constructible_v<T, int> (1)
struct baz
{
    baz() = default;
    //template<typename = void> // (2)
    baz(int) requires std::is_constructible_v<T, int> {}
};

struct foo
{
    struct bar {
        bar() = default;
        bar(foo&);    // (3)
        bar(int);
    };
    baz<bar> m_bars; // (4)
};

int main()
{
    foo a; (void)a;
}
