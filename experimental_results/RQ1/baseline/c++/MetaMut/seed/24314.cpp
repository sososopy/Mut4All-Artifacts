
struct S {
  constexpr S() { }
};

constexpr S s[][1] = {
    { S() }
};

