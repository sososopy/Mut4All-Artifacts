
template <typename T>
struct declval_helper
{
    static constexpr bool stop = false;
    static T value();
};

template <typename T>
T
declval()
{
    static_assert(declval_helper<T>::stop, "declval must not be used");
    return declval_helper<T>::value();
}

template <typename T>
struct A {
    static void foo(int);
};

void bar(int);

template <typename T>
    requires requires {
        bar(declval<T>());
    }
void foo(T t) {
}

class Foo {

    public:

    template <typename T>
        requires requires {
            bar(declval<T>());
        }
    void foo(T t) {
    }

};

void test() {
    Foo f;
    f.foo(5);

    foo(5);
}

