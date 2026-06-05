class A {
  typedef int I; // private member
  template<int> struct Q;
  template<int> friend struct R;
protected:
  struct B { };
};
template<A::I> struct A::Q { };
template<A::I> struct R { };
struct D: A::B, A { };

