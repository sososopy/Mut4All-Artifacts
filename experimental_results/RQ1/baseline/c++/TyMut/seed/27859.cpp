
struct A
{
    [[nodiscard]] static int match() { return 42; }
};

template<typename T>
auto g() -> decltype( T::match(), bool() )
{
    return T::match();
}

int main()
{
    g<A>();
}

