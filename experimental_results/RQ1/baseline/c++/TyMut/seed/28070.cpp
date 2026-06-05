
template<typename T, typename U, typename = U>
struct is_static_castable : std::false_type
{};

template<typename T, typename U>
struct is_static_castable<T, U, decltype(static_cast<U>(std::declval<T>()))> : std::true_type
{};

template<typename To, typename From, std::enable_if_t<is_static_castable<From*, To*>::value, int> = 0>
auto safePtrCast(From* from)
{
    return static_cast<To*>(from);
}

template<typename To, typename From, std::enable_if_t<!is_static_castable<From*, To*>::value, int> = 0>
To* safePtrCast(From* from)
{
    return dynamic_cast<To*>(from);
}

struct BarBase{ virtual ~BarBase() = default;};
struct Bar : virtual BarBase{};

void foo(){
    BarBase* b;
    Bar* b2 = safePtrCast<Bar>(b);
}

