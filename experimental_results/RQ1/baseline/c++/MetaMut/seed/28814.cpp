

struct S{};

struct S2 {
  operator S*();
};

S* f() { return true ? S2{} : nullptr; }
