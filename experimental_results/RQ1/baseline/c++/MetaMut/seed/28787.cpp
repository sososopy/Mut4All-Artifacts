
struct fixed_string { consteval auto size() const { return 42; } };

template<fixed_string s>
static void VerifyHash() {
  [](auto){ s.size(); };
}

void foo() { VerifyHash<{}>(); }

