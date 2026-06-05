
#include <iostream>
#include <type_traits>

struct S {
    template <typename T> explicit S(T) noexcept requires std::is_signed<T>::value {
        std::cout << "T\n";
    }
    template<typename T> explicit S(const T&) noexcept {
        std::cout << "const T&\n";
    }
};

int main() {
    S s(4);
}

