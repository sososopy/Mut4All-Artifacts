template <class T, class V> struct C
{
  template <class... U>
  constexpr C (...) : c { static_cast<U &&>(0)... } {}
  constexpr const V &operator[](T) { return c[0]; }
  V c[T::F];
};
enum D { E, F };
struct A
{
  struct G
  {
    typedef int *H;
    int g, h;
    C<D, H> i;
    constexpr G () : g (), h (), i{} {}
    constexpr G foo (H) { return G (0, 0, 0, 0, i[E]); }
    constexpr G (int, int, H, H, H) : g (), h (), i{} {}
  };
};
struct B : A
{
  static constexpr auto b = G ().foo (0);
};

