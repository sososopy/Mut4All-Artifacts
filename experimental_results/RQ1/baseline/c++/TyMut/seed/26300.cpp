
template<typename T>
struct tuple
{
    T t;
};

template<class... Types>
tuple<Types&&...> forward_as_tuple(Types&&... t) noexcept
{
    return tuple<Types&&...>{ static_cast<Types&&>(t)... };
}

template <typename... Args>
void fooImpl(tuple<Args...> args)
{
}

template <typename... Args>
void foo(tuple<Args...> args)
{
    fooImpl(args);
}

enum E { id };

int main()
{
    foo(forward_as_tuple(id));
    return 0;
}
