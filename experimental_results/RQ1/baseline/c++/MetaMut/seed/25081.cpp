

template < class T, class T >  // should be: template < class T, class S >
class A
{
public:
  explicit A (T (S::*f) ()) {}
};

template < class T, class S > 
A < T, S > foo (T (S::*f) ())
{
  return A < T, S > (f);
}

class B
{
public:
  void bar () {}
};

int
main ()
{
  foo (&B::bar);
  return 0;
}
