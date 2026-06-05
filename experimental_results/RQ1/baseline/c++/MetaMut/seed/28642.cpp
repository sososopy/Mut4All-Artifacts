
template <int __v> struct integral_constant {
  static constexpr int value = __v;
};
struct _Bit_iterator_base {
  template <class> struct basic_symbol {
    basic_symbol() noexcept(integral_constant<false>::value);
  };
  struct symbol_type : basic_symbol<int> {
    symbol_type() : basic_symbol() {}
  };
};
