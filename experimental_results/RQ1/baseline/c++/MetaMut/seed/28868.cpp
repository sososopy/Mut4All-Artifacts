
void not_constexpr() {}

static constexpr int constexpr_loop() {
  for (int i = 0; i < 1; ++i) {
    return i;
  }

  // builds fine with just this:
  // __builtin_unreachable();
  // but not with this:
  not_constexpr();
}

constexpr unsigned i = constexpr_loop();
