
#include <utility>

template<typename ... B>
struct Merged : B ... {
       template<typename ... T>
       Merged(T&& ... t) : B(std::forward<T>(t))... {}

    using B::operator()...;
};

template<typename ... T>
Merged(T...) -> Merged<std::decay_t<T>...>;

int main() {
    const auto l1 = []() { return 4; };
    const auto l2 = [](const int i) { return i * 10; };

    Merged merged(l1,
                  l2,
                  [](const double d) { return d * 3.2; });

    return 0;
}

