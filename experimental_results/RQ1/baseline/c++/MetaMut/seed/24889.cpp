#include <variant>

struct A
{
    int i;
};

struct Getter
{
    Getter(const A & a) : var(&a) {}
    const int & operator ()(const A * a) const { return a->i; }
    const int & value() const { return std::visit(*this, var); }
    std::variant<const A *> var;
};

int main()
{
    A a { .i = 1 };
    const auto & i = Getter{ a }.value();
    return i;
}

