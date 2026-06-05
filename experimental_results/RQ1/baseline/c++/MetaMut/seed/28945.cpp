
template <typename T> struct A { struct B { }; };
template <typename T> struct C { friend struct A <C>::B; };
