
namespace std {
template <typename T>
struct remove_reference {
  using type = T;
};

template <typename T>
constexpr typename std::remove_reference<T>::type &&
move (T &&t) noexcept
{
  return static_cast<typename std::remove_reference<T>::type &&> (t);
}
}

consteval int
f_eval ()
{
  return 0;
}

struct Copy {
  int (*ptr) ();

  constexpr Copy (int (*p) () = nullptr) : ptr (p)
  {
  }
};

Copy *
test ()
{
  return new Copy (std::move (Copy (&f_eval)));
}

