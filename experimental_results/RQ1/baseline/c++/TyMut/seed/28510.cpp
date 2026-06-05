
template <class a, class b>
  concept t = requires(a aa, b bb) { aa == bb; };

template <class I2>
struct I {
  using value_type = int;
  using difference_type = int;

  value_type& operator*() const;
  I& operator++();
  I operator++(int);

  template<t<I2> S>
  friend bool operator==(I, S);
};

static_assert(t<int*, I<int*>>);
