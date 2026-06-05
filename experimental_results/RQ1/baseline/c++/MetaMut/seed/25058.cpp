#include <type_traits>

template <class Fn>
void oops(Fn) {
  static_assert(!std::is_literal_type<Fn>::value, "oops");
}

int main() {
  oops([]{});
}

