
template<class T>
class A {
 public:
  template<int I, class S>
    requires I > 0
  friend int f(const A<S>&);
 private:
  int x = 2;
};

template<int I, class S>
  requires I > 0
int f(const A<S>& a) { 
  return a.x;
} 

int main() { 
  A<double> a;
  f<2>(a);
  return 0;
} 
