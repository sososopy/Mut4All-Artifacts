
struct B {
    int value;
    // auto operator<=>(const base_dimension&) = default;
    friend constexpr bool operator==(const B& lhs, const B& rhs) noexcept { return lhs.value == rhs.value; }
};

//works
template<int i1, int i2>
  requires (i1 == i2)
struct A {};

// works
template<B b1, B b2, bool BB = (b1 == b2)>
struct C {
    static_assert(b1 == b2);
};

// error
template<B b1, B b2>
  requires (b1 == b2)
struct D {};
