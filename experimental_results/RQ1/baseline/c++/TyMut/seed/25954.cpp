
template <typename T>
struct S {
  S() = default;
  S(S&);
};

template <typename T>
S<T>::S(S<T>&) = default;

void f() {
  S<int> s;
  S<int> s2(s);
}

