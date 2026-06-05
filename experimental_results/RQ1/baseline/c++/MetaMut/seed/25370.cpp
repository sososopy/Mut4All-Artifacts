
#include <type_traits>
#include <utility>

template <class F>
struct y_combinator {
    F f;

    template <class... Args>
    decltype(auto) operator()(Args&&... args) const
    {
        return f(*this, std::forward<Args>(args)...);
    }
};
template <class F>
y_combinator<std::decay_t<F>> make_y_combinator(F&& f) {
    return {std::forward<F>(f)};
}

int main() {
    auto factorial = make_y_combinator([](auto fact, int n) -> int {
        if (n == 0) return 1;
        else        return n * fact(n - 1);
    });
    factorial(5);
}
