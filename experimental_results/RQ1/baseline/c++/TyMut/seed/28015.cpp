
struct Z {
  int y;
};

template <class T>
constexpr Z f(const T *data) {
  Z z;
  __builtin_memcpy(&z, data, sizeof(z));
  return z;
}

constexpr Z g(const char *data) { return f(data); }
