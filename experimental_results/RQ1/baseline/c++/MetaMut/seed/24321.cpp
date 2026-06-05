
#include <string>

template <typename S>
class base {
public:
  using string_type = S;
};

template <typename S>
class derived : public base<S> {
  using super_ = base<S>;
public:
#if 0
  /* These work */
  //using string_type = S;
  using string_type = typename super_::string_type;
#else
  /* This does not work */
  using typename super_::string_type;
#endif
  using size_type = typename string_type::size_type;

  size_type big () const {
    return string_type::npos;
  }
};

size_t checkit ()
{
  derived<std::string> x;
  return x.big();
}
