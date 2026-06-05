

struct S
{
    template <typename T, typename U = int>
    U foo_impl(T);

    template <typename T>
    auto foo(T t) -> decltype(foo_impl(t));
};

int main()
{
    S s;
    s.foo(0);
}


