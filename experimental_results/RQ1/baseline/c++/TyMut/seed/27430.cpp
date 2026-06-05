
template <typename To, typename From>
constexpr To
bit_cast (const From &from)
{
  return __builtin_bit_cast (To, from);
}
struct J
{
  long int a, b : 11, h;
};
struct K
{
  long int a, b, c;
  constexpr bool operator == (K x)
  {
    return x.a;
  }
};
static_assert (bit_cast <K> (J{}) == K{}, "");


