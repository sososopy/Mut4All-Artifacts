template<typename T> struct S { };
template<typename T> requires { typename T::type; } struct S<T> { };

