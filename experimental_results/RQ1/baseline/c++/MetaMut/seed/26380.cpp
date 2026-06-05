
template<class>
struct iterator_traits
{};

template<class T>
  requires requires(T __t, T __u) { __t == __u; }
struct iterator_traits<T>
{};

template<class T>
concept C = requires { typename iterator_traits<T>::A; };

struct unreachable_sentinel_t
{
  template<C _Iter>
  friend constexpr bool operator==(unreachable_sentinel_t, const _Iter&) noexcept;
};

template<class T>
struct S
{};

static_assert(!C<S<unreachable_sentinel_t>>);
