
export module mod;

struct X {
  bool b = true;
  constexpr ~X() { }
};

export constexpr X f() { return {}; }

import mod;
static_assert(f().b);
int main() { }
