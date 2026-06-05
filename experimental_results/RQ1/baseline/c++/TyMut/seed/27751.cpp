
struct Value {
  Value() : v{new int{42}} {}
  int* v;
};

struct S {
  static constinit inline Value v{};
};

int main() { return *S::v.v; }
