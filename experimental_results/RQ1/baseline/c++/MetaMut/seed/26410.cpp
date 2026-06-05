
struct X
{
    consteval X(int) {};
};

struct T : X
{
    using X::X;
};

constexpr T t(22);
