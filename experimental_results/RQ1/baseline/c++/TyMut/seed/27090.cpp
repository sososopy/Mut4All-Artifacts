
int main() {
  unsigned const nsz = 0;
  auto repeat_conditional = [&](auto) {
    auto new_sz = nsz;
  };
  repeat_conditional(1);
}
