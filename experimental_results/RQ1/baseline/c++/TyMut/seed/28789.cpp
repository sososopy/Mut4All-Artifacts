
template<bool B> struct S {
  S() {}
  S(S<false>) requires B {}
};
S<false> sf;
