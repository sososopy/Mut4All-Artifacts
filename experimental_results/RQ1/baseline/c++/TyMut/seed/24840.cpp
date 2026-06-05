struct S { int a, b; };

template <int N>
constexpr struct S var[16] = {};

template <>
constexpr inline struct S var<6>[8] = {
  { 1, 1 }, { 2, 0 }, { 3, 1 }, { 4, 0 },
  { 5, 1 }, { 6, 0 }, { 7, 1 }, { 8, 0 }
};

[[gnu::noipa]] void
foo (S)
{
}

template <int N>
void
bar (int x)
{
  foo (var<N>[x]);
}

volatile int x;

int
main ()
{
  bar <6> (x);
}

