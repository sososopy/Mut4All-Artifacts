
struct A{
  void f(){}
};

struct B{
  static A& n(){
    return *new A();
  }
};

template <typename T>
struct C{
  void g(){
    T::n().template f();
  }
};

int main(){
  (new C<B>())->g();
}

