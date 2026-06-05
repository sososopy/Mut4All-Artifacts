

template<typename T>
struct A {
  T f();
};

template<typename T, typename Enable=void> struct B {};
template<typename T> struct B<T, typename A<T>::type> {};

struct C {
  virtual ~C() = 0;
};

int main() {
  B<C>();
}


