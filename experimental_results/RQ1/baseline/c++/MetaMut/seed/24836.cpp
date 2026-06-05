template<typename T> struct A {
    A(T);
};

A(int) -> A<char>;

template<typename T>
using B = A<T>;

A a2(100);
B b2(100);

using A2 = A<char>;
using A2 = decltype(a2); // works

using B2 = B<char>;
using B2 = decltype(b2); // fails, deduces A<int>