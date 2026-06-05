
template<typename T>
T constant {};

template<typename T>
struct foo {
    int operator()() const
    { return 3; }
};

template<typename T>
auto& f = constant<foo<T>>;

int main()
{
    // fine
    auto& ref = f<int>; ref();

    // error: f<int> cannot be used as a function
    f<int>();
}
