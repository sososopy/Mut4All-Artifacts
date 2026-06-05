
template<class T>
struct A {
  A<T>();           // error: simple-template-id not allowed for constructor
  A(int);           // OK, injected-class-name used
  ~A<T>();          // error: simple-template-id not allowed for destructor
};

