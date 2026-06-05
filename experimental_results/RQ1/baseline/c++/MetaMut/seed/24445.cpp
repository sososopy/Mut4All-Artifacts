
template <typename T> struct S {
  T i{[this] {}};
};

S<int> s;

