

struct Z
{
  Z () = default;
  Z (Z const &) = default;
  constexpr Z (Z &&) {}
};

template < typename T > 
constexpr int 
fn (T v)
{
  return fn (v);
}

auto t = fn (Z ());
