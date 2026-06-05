

template<typename T, typename U>
constexpr bool Has_difference_type()
{
  return origin::Same<origin::Difference_type<T>, U>();
}

struct S
{
  using difference_type = int;
};
