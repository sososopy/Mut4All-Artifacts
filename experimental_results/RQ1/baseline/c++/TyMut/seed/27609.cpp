
  #include <memory>
  #include <cassert>

  // error disappears if E doesn't inherit from B
  struct B {};
  struct E : B {};

  struct counter {
    constexpr counter() = default;
    constexpr void inc() {
      size++;
    }

    // error disappears if you remove or reorder this value
    int unused = 0;
    int size = 0;
    [[no_unique_address]] E empty = {};
  };

  constexpr int test() {
    counter x;
    x.inc();
    return x.size;
  }

  int main() {
    assert(test() == 1);        // works, unless optimisations enabled
    static_assert(test() == 1); // fails, always
  }

