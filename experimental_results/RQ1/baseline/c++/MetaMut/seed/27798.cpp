
template<typename T1, typename T2>
struct Y {
  static constexpr bool value = false;
};

template<>
struct Y<void, void> {
  static constexpr bool value = true;
};

template<int x>
struct X {
  static_assert(Y<void, decltype(x)>::value, "1");
};

int main() {
}
