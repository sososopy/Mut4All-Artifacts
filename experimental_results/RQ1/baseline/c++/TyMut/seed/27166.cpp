
template <typename T>
concept C =  requires (T t)
{
  requires t.some_const < 2
#if BUG
  || requires { t.some_fn (); }
#endif
  ;
};


template <unsigned, unsigned>
struct c
{};


template <typename T>
concept P = requires (T t, c <0, 1> v)
{
#if BUG
  { t (v) };
#else
  true;
#endif
};

template <P auto, P auto ...>
struct m
{
  constexpr auto operator () (C auto) const
  {};
};


struct pc
{
  constexpr auto operator () (C auto) const
  {};
};


constexpr auto cc = pc {};	
constexpr auto mmcc = m <cc> {};

