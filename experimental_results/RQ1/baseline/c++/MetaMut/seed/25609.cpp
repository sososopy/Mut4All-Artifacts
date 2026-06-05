
template <class T> struct B : public A {};
template <class T> struct C : public B<T> {
  C(int &&) : _D(::f(0)) {}
};
C<char> g() { return 0; }
