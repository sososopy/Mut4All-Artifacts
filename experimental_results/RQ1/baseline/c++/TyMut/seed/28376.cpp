#include <concepts>
template<class T>
struct B {
    int f() requires (!std::same_as<T, char>) {
        return 0;
    }
    int f() requires (!std::same_as<T, char> && !std::same_as<T, float>) {
        return 1;
    }
};
template struct B<int>;

