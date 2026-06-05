
template<class T>
void f() {
  constexpr int dim = 1;
  auto f = [&] {
    int n[dim * 1];
  };
}

template void f<int>();

