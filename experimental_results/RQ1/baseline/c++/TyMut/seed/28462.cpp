
template<class>
template<class T>
concept C = requires { typename T::type; };

template <C> class S {};

S<void> s;

