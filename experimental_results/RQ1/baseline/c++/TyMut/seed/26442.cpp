

#include <format>
#include <iostream>

using namespace std;

constexpr auto f(bool a) -> void {

    if (a) {

        cout << format("hello 1\n");

    } else {

        cout << format("hello 2\n");
    }

}

template <bool T> 
constexpr auto g() -> void {

    if constexpr (T) {

        cout << format("hello 3\n");

    } else {

        cout << format("hello 4\n");
    }
}


int main() {
    f(true);
    f(false);
    g<true>();
    g<false>();

}

