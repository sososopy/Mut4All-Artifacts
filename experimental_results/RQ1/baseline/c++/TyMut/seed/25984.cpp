
struct base {
    using base_type = base;
    using type = int;
};

template<typename T>
struct derived: T {
    using typename derived::base_type::type;

    // Uses of once dependent type, explicitly introduced
    // into scope by using declaration above -- no complaints
    type foo(type);

    // No complaints as well
    static type t;
};

// Error: conflicting declaration
template<typename T>
typename derived<T>::type derived<T>::t = 0;

int main()
{}

