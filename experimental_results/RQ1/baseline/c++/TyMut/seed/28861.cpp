
template <int... E>
void f() {
  [] { enum e { e = E }; };
}

template void f<>();
