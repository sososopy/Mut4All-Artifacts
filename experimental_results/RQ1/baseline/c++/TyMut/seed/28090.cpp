
#include <iostream>
#include <type_traits>

template <class T>
struct test {
    T value;

    template <class F>
    std::enable_if_t<std::is_invocable_v<F&&, T&>>
    fn(F&& op) & { op(value); } // #1

    template <class F>
    std::enable_if_t<std::is_invocable_v<F&&, T const&>>
    fn(F&& op) const& { op(value); } // #2
};

template <class T> test(T) -> test<T>;

int main() {
    test instance{1};
    instance.fn([](auto& i){
        // error: cannot assign to variable 'i'
        // with const-qualified type 'const int &'
        i = 0; 
    });
}
