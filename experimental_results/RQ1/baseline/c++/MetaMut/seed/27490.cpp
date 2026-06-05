
#include <type_traits>

class Const {
 public:
  int const i_;

  Const(int i) : i_(i) {}

  Const& operator=(Const& rhs) = default;
};

int main(int argc, char**) {
  Const c{argc};
  static_assert(!std::is_assignable<Const, Const>::value,
                "should not be able to assign i_");
#ifdef ASSIGN
  Const d{argc + 1};
  d = c;
#endif
  return c.i_;
}

