
struct thing {
    void foo() {}
};

template<typename>
constexpr int count()
{
    auto item = thing {};
    for(; (item.foo(), false););
    return 0;
}

int main()
{
    static_assert( count<int>() == 0, "" );
}
