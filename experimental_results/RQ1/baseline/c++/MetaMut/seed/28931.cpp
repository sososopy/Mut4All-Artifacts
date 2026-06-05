
#include<type_traits>
template<class T>struct A{T t;};
A(double)->A<void*>;
A a{1};
static_assert(std::is_same_v<decltype(a),A<int>>);
