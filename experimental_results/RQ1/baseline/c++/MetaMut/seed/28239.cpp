
template<auto> struct dummy;

template<typename T>
concept C = requires(T v) {
    v.attr1;
    typename dummy<T::attr2>;
    v.fun1();
    T::fun2();
};

