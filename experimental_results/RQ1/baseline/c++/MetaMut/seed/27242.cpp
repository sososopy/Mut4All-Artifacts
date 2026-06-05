
struct foo
{
    constexpr foo() noexcept = default;

    // fails with gcc10; commenting : foo{} out works with gcc10
    constexpr foo(int a, int b) : foo{} 
    {
        bar += a + b;
    }
    int bar{}; 
};

int main()
{
    constexpr foo bar{1, 2};
}
