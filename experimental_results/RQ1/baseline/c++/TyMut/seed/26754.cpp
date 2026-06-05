
template<class X>
concept bool C() {
  return requires(X x, bool b) {
    requires b;
    x++;
  };
}

int main() {
  C<int>();
  return 0;
}
