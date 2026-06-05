
struct NonCopyable {
  NonCopyable(int) {}
  NonCopyable(const NonCopyable&) = delete;
};

struct S {
  S(int i) : val(NonCopyable{1}) {}

  [[no_unique_address]] NonCopyable val;
};

struct T {
  T(int i) : val(NonCopyable{1}) {}

  NonCopyable val;
};

