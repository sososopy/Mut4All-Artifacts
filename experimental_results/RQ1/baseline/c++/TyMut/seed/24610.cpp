
struct A
{
  template<typename T> A(T) = delete;
};

template<> A::A<int>(int) {}

A a(0);
