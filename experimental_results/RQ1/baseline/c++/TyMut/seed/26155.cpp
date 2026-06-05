
template <class T>
struct X {
    struct Y {
        enum class E : T;
    };
};

template <class T>
enum class X<T>::Y::E : T { e1, e2 };

template class X<int>;
