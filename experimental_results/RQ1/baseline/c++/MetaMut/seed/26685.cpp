
#include <type_traits>

struct any_
{
    template <class R> any_(R&&) {
        static_assert(std::is_lvalue_reference<R>::value, "expected lvalue ref");
    }
};

int main()
{
    const int i = 7;
    any_ oi = i;
}
