
template<typename...> struct A {};

template<typename T> struct B
{
  typedef A<__bases(T)...> C;
};

struct X {};
struct Y : X* {};

B<Y> b;
