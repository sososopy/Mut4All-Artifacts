
template<typename T, unsigned N>
struct array
{
  T data[N];

  constexpr T& operator[](unsigned n) { return data[n]; }
};

enum class EnumC : signed char { Forward = 1, Backward = -1 };

array<unsigned char, sizeof(EnumC)> buf;

template <typename T>
struct S {
  void foo() { auto const cast_to_enum{static_cast<EnumC>(buf[0])}; }
};

S<int> s;
