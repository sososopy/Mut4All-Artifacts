template<class>
concept C = true;

auto apply(auto f, auto... args) {
  f(args...);
}

auto f(auto) {
  apply(
    [](auto... x) {
      ([] { if constexpr (C<decltype(x)>); }(), ...);
    }, 0);
}

int main() {
  f(0);
}

