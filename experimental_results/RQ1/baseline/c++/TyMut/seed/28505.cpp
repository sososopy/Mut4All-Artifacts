
int main() {
  int x = 1;
  auto(1); // ok
  static_cast<void>(auto(x)); // ok
  auto{x}; // ok
  auto(x); // error
}

