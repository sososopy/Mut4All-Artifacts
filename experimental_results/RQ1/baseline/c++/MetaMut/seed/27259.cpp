
using size_t = decltype(sizeof(0));

template<typename T> struct remove_reference { using type = T; };
template<typename T> struct remove_reference<T&> { using type = T; };
template<typename T> struct remove_reference<T&&> { using type = T; };
template<typename T> using remove_reference_t = typename remove_reference<T>::type;

struct seconds
{
  long count() const;
};

template <typename T>
struct hash
{
};

template <>
struct hash<seconds>
{
  auto operator()(seconds const cp) const noexcept
  {
    return (size_t)cp.count();
  }
};

template <typename ...T>
size_t hash_combine(T&& ...v) noexcept(noexcept(
  ((hash<remove_reference_t<T>>()(v)), ...)))
{
  auto seed{672807365};

  (
    (
      seed ^= hash<remove_reference_t<T>>()(v) +
        0x9e3779b9 + (seed << 6) + (seed >> 2)
    ),
    ...
  );

  return seed;
}

template<typename T>
struct tuple_node
{
  T t;
};

template<typename... T>
struct tuple : tuple_node<T>...
{
};

template<typename F, typename... T>
auto
apply(F& f, tuple<T...> t)
{
  f(static_cast<tuple_node<T>&>(t).t...);
}

template <typename ...T>
struct hash<tuple<T...>>
{
  auto operator()(tuple<T...>& t) const
  {
    return apply(hash_combine<T&...>, t);
  }
};

int main()
{
  using T = tuple<seconds>;
  hash<T> h;
  T t;
  h(t);
}
