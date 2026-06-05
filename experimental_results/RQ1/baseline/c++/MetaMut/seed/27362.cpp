

template <typename T> class A {
  A(int, int);
  template <typename> friend class A;
  friend T;
};

template<typename U> struct B {
  template<typename T> struct C {
    A<B> begin() { return {1, 0}; }
  };
  template<typename T, int *P = nullptr>
  C<T> fn();
};

int
main ()
{
  B<int> b;
  b.fn<int>().begin();
}
