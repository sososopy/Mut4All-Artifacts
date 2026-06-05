
void f(unsigned long long);

template<typename T> struct X {
  friend void f(unsigned long long) {
     T t;
  }
};

int main() { f(1234); }
