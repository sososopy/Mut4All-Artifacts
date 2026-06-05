
    constexpr int
    raise(const int x, const unsigned n) noexcept
    {
      switch (n)
        {
        case 0:
          return 1;
        case 1:
          return x;
        default:
          const auto m = (n >> 1);
          const auto y = raise(x, m);
          return ((m << 1) == n) ? y * y : x * y * y;
        }
    }
    
    static_assert(raise(3, 2) == 9, "");
    static_assert(raise(2, 3) == 8, "");

