
struct S {
};

template<class T>
struct TS {
        __attribute__((visibility("default")))
        static int i;
        __attribute__((visibility("default")))
        static S s;
};

template<class T>
int TS<T>::i{};

template<class T>
S TS<T>::s{};

template struct TS<int>;
template struct TS<S>;
