
template <typename> struct B
{
  B (int = A()) {}
  template <typename ...> struct A;
};


template <class T> struct A
{
  template <T ...> struct B;
  A () { B c; };
};

