struct aggressive_aggregate
{
    int a;
    int b;
};

int main()
{
    static_assert(__is_constructible(aggressive_aggregate, int, int)); // fails
    decltype(aggressive_aggregate(1,2)) foo; // ill-formed
    bool b = noexcept(aggressive_aggregate(1,2)); // ill-formed
}
