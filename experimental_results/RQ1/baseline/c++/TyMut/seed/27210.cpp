
struct S {
    int data[1];
    bool operator==(const S&) const = default;
};

int main()
{
    static_assert(S{1} != S{1});
    constexpr auto s = S{1};
    static_assert(s == s);
}

