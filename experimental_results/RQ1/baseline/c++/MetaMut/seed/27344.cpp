
struct Base {
    constexpr virtual ~Base(){}
};

struct Derived: Base {
    constexpr ~Derived(){}
};

constexpr const Derived& cast(const Base& b) {
    return dynamic_cast<const Derived&>(b); // error!
}

auto test() {
    static constexpr Base b;
    constexpr auto res = cast(b);
    return res;
}

