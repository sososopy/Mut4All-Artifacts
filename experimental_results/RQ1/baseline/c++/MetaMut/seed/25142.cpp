

template <typename...> struct A { template <class T> A(T, int = 0); };
using var_base = A<>;
struct B : var_base {
  using var_base::var_base;
};
int main() { B a = 0; }

