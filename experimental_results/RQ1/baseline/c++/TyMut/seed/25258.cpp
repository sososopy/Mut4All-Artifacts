

template<typename>
constexpr bool foo = true;

template<>
constexpr bool foo<int> = false;

template<typename T>
struct non_member {};

template<typename T>
    requires foo<T>
struct non_member<T> {
    static_assert( foo<T> );
};

template<typename>
struct enclosing {
    template<typename T>
    struct member {};

    template<typename T>
        requires foo<T>
    struct member<T> {
        // error: static_assertion failed
        static_assert( foo<T> );
    };
};

int main()
{
    // fine
    non_member<int> {};

    // blows up
    enclosing<void>::member<int> {};
}
