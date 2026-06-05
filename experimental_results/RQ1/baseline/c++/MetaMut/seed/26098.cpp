
template <typename T>
T declval();

template <class A>
auto a(A) -> decltype(declval<[]() {}>) {}

void fun() {
  a(0);
}

