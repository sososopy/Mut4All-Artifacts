
#include <iostream>
#include <vector>


struct Yes { char x[2]; };
struct No { char x; };

template <typename T>
constexpr auto f(const T& t) -> decltype(t.end()-t.begin(), Yes{});

constexpr No f(...);

std::vector<int> v;
bool vecHasIteratorDiff = sizeof(f(v)) == sizeof(Yes);
bool intHasIteratorDiff = sizeof(f(3)) == sizeof(Yes);
int main() {
    int x;
    std::cout << vecHasIteratorDiff << std::endl;
    std::cout << intHasIteratorDiff << std::endl;
    return 0;
}
