

template <class R>
constexpr bool has_customization = false;
template <class R>
requires
  requires(R&& r) { iter_move((R&&)r); }
constexpr bool has_customization<R> = true;

template <class C>
struct basic_iterator
{
  C pos_;

  // template <int=42>
  friend decltype(auto) iter_move(const basic_iterator& i)
  requires
    requires { i.pos_.move(); }
  {
    return i.pos_.move();
  }
};

static_assert(!has_customization<basic_iterator<int>>);

