
template<class T>
struct A {
  template<class U> struct B { B(U); };
};

template<class T>
void f() {
  typename A<T>::B x(0);
}

template void f<int>();
