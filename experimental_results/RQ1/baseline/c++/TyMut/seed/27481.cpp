
struct base0 {
    int x;
    base0() = default;
    constexpr base0(int x) : x{x} { }
};

struct base1 : base0 {
    using base0::base0;
    constexpr base1(void*) : base0{} { }
};

struct derived : base1 { using base1::base1; };

void f() { []() consteval { derived{}.x + 1; }(); }
