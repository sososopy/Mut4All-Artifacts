
int main() {
  const char x[] = R"tag(good)tag";
#if 0
  const char y[] = R"t@g(bad)t@g";
  const char z[] = R"t$g(bad)t$g";
#endif
  return x[0];
}

