#include <concepts>
template<class T> void f(T x) {
    std::same_as<T> auto y = x;
}
int main() { f(1); }

