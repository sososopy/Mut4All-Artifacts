
struct Base {
    constexpr virtual ~Base(){}
};

struct Derived: Base {};

constexpr Derived d;

