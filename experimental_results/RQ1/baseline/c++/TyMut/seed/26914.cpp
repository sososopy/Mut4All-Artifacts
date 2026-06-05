
struct Ptr {
  int* p;

  constexpr Ptr(int* p) noexcept : p{p} {}
  constexpr int& operator*() const {
    return *p;
  }
};

constexpr int f(int& i) {
  //Ptr first{&i}; // Works.
  Ptr first = &i;  // Error
  return *first;
}

constexpr int g() {
  int i = 42;
  return f(i);
}

static_assert(g() == 42);

