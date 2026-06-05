
template <int>
void a() {
  int b;

  [&](auto c) {
    if constexpr (sizeof(c) != 42) {
      [&] { b; };
    }
  }(0);
}

template void a<1>();
