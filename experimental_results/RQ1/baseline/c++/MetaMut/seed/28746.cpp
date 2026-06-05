
template<class T> concept C =
  requires { typename T::base; };

struct base { };

static_assert(C<base>);
