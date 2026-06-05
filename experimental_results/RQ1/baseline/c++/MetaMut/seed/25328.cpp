
extern "C" void printf(...);
struct A {
  A(int, char *p2) { printf(p2); }
};
template <int, typename> struct B { static A static_var; };
template <int LINE, typename GETTER>
A B<LINE, GETTER>::static_var{0, GETTER::get()};
struct C {
  void unused() {
    static char function_static;
    struct D {
      static char *get() { return &function_static; }
    };
    auto addr = B<0, D>::static_var;
  }
};
int main() {}
 
