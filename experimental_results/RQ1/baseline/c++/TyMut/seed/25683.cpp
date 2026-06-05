
  struct A { };

  static_assert(({ 1; }), "oops");
  constexpr auto a = ({ A(); });
