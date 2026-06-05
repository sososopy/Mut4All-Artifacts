
template<class T>
struct A {};

template<class T>
struct B1 : A<T> {
  typedef A<T> super_t;
  using A<T>::A; // #7
};

template<class T>
struct B2 : A<T> {
  using A<T>::A;
  typedef A<T> super_t; // #13
};
