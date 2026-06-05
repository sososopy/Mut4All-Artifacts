
template <int* format>
struct g { g() { *format =1; } };

template <int>
void impl() {
    static int i=1;
    static g<&i> h{};
}

int main() {
  impl<0>();
  return 0;
}
