
typedef int T __attribute__((vector_size(8)));

T foo(const T& a, const T& b)
{
  constexpr T c = a < b;
  return c ? a : b;
}
