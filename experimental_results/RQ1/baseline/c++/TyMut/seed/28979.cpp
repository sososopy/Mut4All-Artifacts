
template <typename... a>
void f (a... n)
{
  do; while (--n); {}
}
void g ()
{
  f(3);
}


template <typename... a>
void f (a... n)
{
  do; while (n--); {}
}
void g ()
{
  f(1,2);
}

