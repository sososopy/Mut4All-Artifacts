struct A
{
    _BitInt(64) a;

    constexpr bool operator==(A const& a) const = default; // implicitly deleted due to _BitInt(64)
};

bool cmp(A a, A b)
{
    return a == b; // error
}