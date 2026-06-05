
template <int>
void f() {
  []<int>() {
    #pragma GCC unroll 9
    for (int i = 1; i; --i) {
    }
  };
}

int main() {
  f<0>();
}
