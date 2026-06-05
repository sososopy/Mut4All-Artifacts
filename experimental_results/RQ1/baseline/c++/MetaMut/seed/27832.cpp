
template <int> struct S;

template <S> struct W {
  template <typename> static int foo();
  bool b = foo<int>();
};
