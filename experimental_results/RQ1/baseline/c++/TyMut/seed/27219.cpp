
struct b {
  constexpr b() : c() {}
  [[no_unique_address]] struct a {} c;
};
struct d {
  constexpr d() : c() {}
  int c;
};
struct e : b, d {};
class f {
  float g = 1.0f;
  e h;
} i;

