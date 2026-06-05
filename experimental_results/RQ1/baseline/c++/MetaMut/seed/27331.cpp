
  constexpr bool test() {
      auto i = 1 << 132;
      return true;
  }

  static_assert(test());

