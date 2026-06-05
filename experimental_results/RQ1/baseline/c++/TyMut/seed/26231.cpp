template<typename T1, typename T2>
struct Foo {};

struct Bar {
    void f(Foo<int, int> = Foo<int, int>());
};

