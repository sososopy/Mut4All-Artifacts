
int main() {
  int a {10};
  auto [b]{ [&a](){} };
  return b - a;
}

