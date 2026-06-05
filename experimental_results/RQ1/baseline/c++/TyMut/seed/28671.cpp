

#include<type_traits>
template<unsigned int N, class T>
void f(const T[N]){}

template<unsigned int N, class T>
using fPtr=decltype(f<N,T>)*;

template<unsigned int N, class T>
fPtr<N,T> af[N]={&f<N,T>}; // even without initialization,error still is

template<unsigned int N, class T>
void g(const decltype(af<N,T>)&){}

static_assert(std::is_same<decltype(af<1,int>),
 fPtr<1,int>[1] >::value, "af is correct"); // #1

static_assert(std::is_same<decltype(g<1,int>),
 void(fPtr<1,int>const(&)[1])>::value, "fun"); // #2

template<>
void g<1,int>(fPtr<1,int>const(&)[1]){}


