
#include <vector>
#include <array>

template <typename T, std::size_t n>
class StaticVector
{
 public:
    StaticVector() = default;
    void foo(){}

 private:
    std::array<T, n> data{};
};

class Foo
{
    StaticVector<std::vector<std::size_t>, 40000> data_{};
};

int main()
{
    Foo f;
    return 0;
}

