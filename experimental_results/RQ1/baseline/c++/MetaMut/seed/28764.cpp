
template <class... Ts>
auto f(Ts...) {
  [] { enum class e : Ts {}; };
}

int main() {
  f(0, true);
}
