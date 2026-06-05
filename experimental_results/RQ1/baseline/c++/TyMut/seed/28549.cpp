
#include <cassert>

struct test {
  enum class state { off, on };
  state st: 1; // bit field
  bool is_on() const {
    return st == state::on; // this is ok
  }
  template<bool flag>
  bool is() const {
    if constexpr (flag) return st == state::on; // error?
    else return st == state::off;
  }
};

int main() {
  test t{test::state::on};
  return !t.is<true>();
}

