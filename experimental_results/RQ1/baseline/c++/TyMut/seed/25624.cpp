
template <int I> struct A {
};
template <long long I, int J> A<I%J> f(A<I>, A<J>) {
  A<I> a;
  return a;
}
A<-9223372036854775808LL> (*g)(A<-9223372036854775808LL>, A<-1>) = &f<-9223372036854775808LL, -1>;
