
#include <cassert>

template <typename T>
struct Foo
{
    T val;
};

class Bar {
   public:
    constexpr Bar() = default;
    constexpr Bar(Bar const& other) { other.val_ = 42; }
    constexpr int val() const { return val_; }
 private:
    mutable int val_{};
};

int main()
{
    constexpr Foo<Bar> x{};
    Foo<Bar> y{x};
    assert(x.val.val() == 42);
}
