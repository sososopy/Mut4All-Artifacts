

#include <memory>

template <typename T>
struct X {
  static __thread T value_;
};

