
template <typename T>
struct Foo {
    T x;
};

template <typename T>
auto mul(T lhs, Foo<T> rhs)
{
    return Foo<T>{ lhs * rhs.x };
}

template <typename T>
auto operator*(T lhs, Foo<T> rhs)
{
    return Foo<T>{ lhs * rhs.x };
}

template <typename T>
T bar(Foo<T> f)
{
    return f.x;
}

int main()
{
    bar(mul(1, Foo<int>{ 2 })); // Works
    bar(1 * Foo<int>{ 2 });     // Fails to compile
}

