template<bool B> struct S { };
int x;
S<&x == &x> s;
