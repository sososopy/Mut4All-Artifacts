
struct ratio {
  int num;
  int den;
};

template<ratio R>
struct X {
  static constexpr auto ratio = R;
};

template<ratio R>
concept positive = R.num > 0 && R.den > 0;

template<typename T>
concept ratio_positive = positive<T::ratio>;

void foo(ratio_positive auto x) {}

void boo()
{
  foo(X<ratio(-1, 2)>());
}
