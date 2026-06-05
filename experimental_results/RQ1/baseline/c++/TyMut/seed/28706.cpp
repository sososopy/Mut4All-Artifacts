

class X { };
template<typename T1, typename T2>
void foo (T1, T2);

struct S {
  [[deprecated]] friend void f(); // error
  [[deprecated]] friend void f2() { }
  friend void f3 [[deprecated]] (); // error
  friend void f4 [[deprecated]] () { }
  [[deprecated]] friend void; // error
  [[deprecated]] friend X; // error
  [[deprecated]] friend class N; // error
  friend class [[deprecated]] N2; // error
  [[deprecated]] friend void foo<>(int, int); // error
  // FIXME: When PR100339 is resolved.
  //[[deprecated]] friend void ::foo(int, int); // error
};

template<typename T>
class node { };

template<typename T>
struct A {
  [[deprecated]] friend T; // error
  [[deprecated]] friend class node<T>; // we warn
  template<typename>
  [[deprecated]] friend class A; // we warn
  template<typename>
  [[deprecated]] friend void bar () { } // OK
  template<typename>
  [[deprecated]] friend void baz (); // error
};
