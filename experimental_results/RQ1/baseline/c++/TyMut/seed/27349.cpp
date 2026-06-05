
#include <tuple>
#include <iostream>

struct Index {
    size_t i;
};

int main() {
    std::tuple<int, float, double, size_t> t{1, 2.f, 3, 4};

    Index i{1};
    std::apply([i](auto&& ...args) {
        (..., [i](auto&& arg) { std::cout << arg + i.i << "\n"; }(args));
    }, t);

}

