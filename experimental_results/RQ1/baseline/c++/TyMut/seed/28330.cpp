
template <typename F_>
void f() {
  decltype(&F_::operator()) p;
  __asm__ __volatile__("__compnerd_was_here__" : [output] "=r" (p) : [input] "r" (&F_::operator()));
}

auto L = [](){};
template void f<decltype(L)>();
