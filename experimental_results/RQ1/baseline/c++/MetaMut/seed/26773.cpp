
#include <array>
#include <iostream>

template<typename T, uint N>
class A
{
    using Arr = std::array<T,N>;
public:
    constexpr A() = default;

    constexpr void displayArr() {
        for (uint n=0; n<N; n++) {
            std::cout << arr_[n];
        }
    }

private:
    static constexpr Arr makeArr() {
        Arr temp{};
        for (uint n=0; n<N; n++) {
            temp[n] = T(n);
        }
        return temp;
    }

    static constexpr Arr arr_ = makeArr();
};

int main() {
    A<double, 5> a;
    a.displayArr(); // If this isn't called - no problems
    return 0;
}
