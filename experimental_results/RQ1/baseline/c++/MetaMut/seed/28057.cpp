
#include <compare> // omitting trigger ICE

template<auto V>
struct A {};

struct B {
    inline constexpr auto operator<=>(const B& rhs) const = default;
//private:
    int value; // why must this member be public?
};

int main() {
    A<B{}> t;
}
