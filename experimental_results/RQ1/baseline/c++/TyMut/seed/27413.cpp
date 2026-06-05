
#include <type_traits>
class Bar { };
struct Foo {
    template <typename P> requires std::is_base_of_v<Bar, P>
    Foo(P const&);
};
template <typename P>
Foo fun(P const& arg) {
    (bool)arg;
    return Foo {arg};
}
int main() {
    fun(Bar{});
    return 0;
}

