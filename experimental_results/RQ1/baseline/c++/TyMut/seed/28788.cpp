

struct base1 { int a; };
struct base2 { int b; };
struct derived : base1, base2 {};

derived& to_derived_bad(base2* b)
{
    return *static_cast<derived*>(b);
}

derived& to_derived_good(base2* b)
{
    return static_cast<derived&>(*b);
}

