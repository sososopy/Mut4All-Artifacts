
template<typename T> struct X {
  T x;
  template< template <typename X> class T> class Y {
    T<void> x;
  };
};
X<int>::Y<X> x;
