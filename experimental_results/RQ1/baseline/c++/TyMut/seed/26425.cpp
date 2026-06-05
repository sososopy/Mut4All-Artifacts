
struct A {
    template<typename>
    using B = int;
};

inline constexpr bool go = requires{typename A::template B<int>;};

