// compile with clang++ -std=c++11 -c example.cc -o /dev/null

#include <memory>
#include <type_traits>

struct P {};

template <typename T>
struct C
{
    C(const std::shared_ptr<const P> & =
        typename std::enable_if<std::is_same<T, int>::value,
                                std::shared_ptr<const P>>::type()) {}

    C(const C &) = delete;
};

int main() {
    C<float> c(std::make_shared<const P>());
    C<int> d;
    return 0;
}

