
template <int>
auto l = []<int x> requires requires { x; } {};

int main() {
  l<0>.template operator()<0>();
}

