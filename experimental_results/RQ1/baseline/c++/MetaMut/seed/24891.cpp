template<class T>
class single;

template<class T, class U>
class check_constructible {
    check_constructible() = default;

    static_assert(is_constructible_v<T, U>);

    template<class X>
    friend class single;
};

template<class T>
struct single {
    template<class U>
    single(U u, check_constructible<T, U> = {})
    : x(u)
    {}

    T x;
};

int main() {
    single<int> x(0);
}