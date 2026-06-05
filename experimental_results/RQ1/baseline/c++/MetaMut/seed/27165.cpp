
template <typename T>
struct S {
    int simple(T::type);

    template <typename U>
    int member(U::type);
};

template <typename T>
int S<T>::simple(T::type) { // we still wrongly require 'typename' here...
    return 1;
}

template <typename T>
template <typename U>
int S<T>::member(U::type) { // ...and here
    return 2;
}

