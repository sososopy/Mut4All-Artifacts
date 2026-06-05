

template <class T>
struct A
{
  A(const T&) {}
  A(const A&) {}
};

template class A<decltype(nullptr)>;
