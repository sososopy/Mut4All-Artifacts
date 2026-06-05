
template <class T> struct C {
    static int x;
};
template <class U> struct S {
    static const int size = 1;
};
template <class T> int C<T>::x(S<T>::size);  // Here `S<T>::size` is treated as a type.
