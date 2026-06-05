
export module internalname;

constexpr int radix_16 = 16;

template <class T, auto Param>
inline auto do_from_chars() -> T {
  if (Param > 4) {
    return 5;
  }
  else {
    return 4;
  }
}

export {
  template <class T> struct parse_number {
    auto operator()() -> T {
      return do_from_chars<T, radix_16>();
    }
  };
}

