
short arr3[1];
template <typename... Ts> struct A {
  template <Ts... Ns> A(Ts (*...qs)[Ns]);
};
A<short> a(&arr3);
