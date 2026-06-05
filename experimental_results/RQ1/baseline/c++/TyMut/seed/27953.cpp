
#include <initializer_list>
#include <utility>
#include <tuple>

template<typename... Funcs>
void invoke_all_of(Funcs&&... f) {
    // initializer_list is special, and guarantees that the ... is expanded in order.
    (void)std::initializer_list<bool>{(void(
        std::forward<Funcs>(f)()
    ), false)...};
}

int main() {
    std::apply([](auto... a) {
        invoke_all_of([&]{
            using A = decltype(a);
        }/*...*/);
    }, std::make_tuple(1));
}


