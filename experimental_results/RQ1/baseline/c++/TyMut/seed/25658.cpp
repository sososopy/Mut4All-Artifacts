
template<int> struct A { A(int); };
template<int...N> struct B {
  template<int...M> struct C : A<N>... {
    C() : A<N>(M)... {}
  };
};
B<1,2,3>::C<4,5> c;
