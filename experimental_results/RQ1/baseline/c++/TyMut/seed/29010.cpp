
struct A
{
    unsigned char foo : 1;
    unsigned char bar : 1;

    constexpr bool operator==(const A&) const = default;
};


int main()
{
    A a{}, b{};

    a.bar = 0b1;

    return a == b;
}


