
template<typename T> using id = T;

int i = 1;
int&& a = id<int[3]>{1, 2, 3}[i];           // temporary array has same lifetime as a
const int& b = static_cast<const int&>(0);  // temporary int has same lifetime as b
int&& c = cond ? id<int[3]>{1, 2, 3}[i] : static_cast<int&&>(0);
                                            // exactly one of the two temporaries is lifetime-extended
like:
template<typename T> using id = T;
struct S { S () { s++; } ~S () { s--; } S (int) { s++; } static int s; };
int S::s = 0;

void
bar (bool cond)
{
  if (S::s != (cond ? 7 : 5))
    __builtin_abort ();
}

void
foo (bool cond)
{
  int i = 1;
  // temporary array has same lifetime as a
  S&& a = id<S[3]>{1, 2, 3}[i];
  // temporary S has same lifetime as b
  const S& b = static_cast<const S&>(0);
  // exactly one of the two temporaries is lifetime-extended
  S&& c = cond ? id<S[3]>{1, 2, 3}[i] : static_cast<S&&>(0);
  bar (cond);
}

int
main ()
{
  foo (true);
  foo (false);
}

