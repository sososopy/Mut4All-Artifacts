
struct Base { template<class T> Base(T) { } };
struct Derived: public Base { using Base::Base; };

int main() {
  Derived(1);
  return 0;
}
