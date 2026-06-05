
struct simd {
  using _Short8 [[gnu::vector_size(16)]] = short;
  _Short8 _M_data;
  constexpr simd(short x) : _M_data{x, x, x, x, x, x, x, x} {}
  constexpr friend unsigned operator==(simd __lhs, simd __rhs) {
    _Short8 __tmp = __lhs._M_data == __rhs._M_data;
    using _UShort = unsigned short;
    auto __bools = __tmp ? _UShort(1) : _UShort(0);
    unsigned __bits = 0;
    for (int i = 0; i < 8; ++i)
      __bits |= __bools[i] << i;
    return __bits;
  }
};
auto f()
{
  constexpr auto tmp = simd(1) == simd(2);
  return tmp;
}
