
template<typename T> struct S{};
auto __attribute__ ((__may_alias__)) a=S<int>{};

