
template<int...> struct A {};

template<> struct A<0>
{
  typedef enum { e } B;
};

A<0> a;
