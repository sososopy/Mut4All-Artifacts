
import<initializer_list>;

constexpr void test() {
  for (const int i : {0, 1, -1}) { }
}
