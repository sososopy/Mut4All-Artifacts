
struct S {
  int x;

  auto foo() {
    return [*this](this auto &self) {
      this->x = 4;
    };
  }
};

