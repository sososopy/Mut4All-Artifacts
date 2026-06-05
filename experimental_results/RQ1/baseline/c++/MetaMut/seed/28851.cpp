
template <class... Ts>
auto f(Ts...) {
  []() requires (sizeof(Ts) == 0) {};
}

int main() {
  f(0);
}
