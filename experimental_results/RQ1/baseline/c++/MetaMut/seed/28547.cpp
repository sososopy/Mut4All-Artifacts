
template <class... T>
struct Foo;

template <class A, class B>
concept C = true;

template <class... T>
struct Foo {
    template <class... U>
        requires ((... && C<T, U>))
    friend void bar(Foo, Foo<U...>) {}
};

int main() {
    Foo<int> x;
    bar(x, Foo<int>());
}
