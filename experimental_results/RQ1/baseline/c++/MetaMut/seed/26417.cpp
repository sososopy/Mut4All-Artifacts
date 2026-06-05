
#include <concepts>
#include <functional>

template <class T>
struct Foo;

template <>
struct Foo<int> {
    template <class U>
    using Type = U;
};

struct S {
    template <class U>
    using Type = U;
};

template <class T>
concept C1 = requires { typename Foo<T>::template Type<bool>; };

template <class T>
concept C2 = requires { typename T::template Type<bool>; };

int main() {
    static_assert(C1<int>);  // Fails with `the required type 'typename Foo<T>::Type' is invalid`
    static_assert(C2<S>);  // Passes.
}
