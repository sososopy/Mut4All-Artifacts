
//$ cat z1.cc
class A {
  ~A() {}
};
template <class> class B {
  A f = [] {};
};
B<int> c;


//$ cat z2.cc
class A {
  ~A() {}
};
template <class T> class B {
  A f = [](T) {};
};
B<int> c;

