struct immovable {
  immovable() = default;
  immovable(immovable &&) = delete;
};

struct S {
  static immovable f() {
    return {};
  }
};

immovable g() { 
  return S().f();
}

