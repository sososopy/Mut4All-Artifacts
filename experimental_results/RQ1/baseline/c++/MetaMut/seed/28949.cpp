
template <class> struct A;
template <class, class B> using C = A<B>;
void *f { C(); }


