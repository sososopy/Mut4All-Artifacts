
#if defined USE_CONSTEXPR
# define CONSTEXPR constexpr
#else
# define CONSTEXPR
#endif

struct A
{
  virtual CONSTEXPR char f () const
  { return 'A'; }
};

struct B : A
{
  char x;

  constexpr B () : x (0)
  { x = ((A *)this)->f(); }

  virtual CONSTEXPR char f () const
  { return 'B'; }
};

struct C : B
{
  virtual CONSTEXPR char f () const
  { return 'C'; }
};

int main ()
{
  CONSTEXPR C c;
  if (c.x != 'B')
    __builtin_abort ();
}
