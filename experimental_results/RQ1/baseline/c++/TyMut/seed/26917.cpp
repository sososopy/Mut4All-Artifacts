
template <class T> concept constexpr bool C = true;

template <class T>
struct A {
  int f() requires C<T> { return 1; }
  int f() requires !C<T> { return 2; }
};

int main() {
  return 0;
}

template struct A<int>;
