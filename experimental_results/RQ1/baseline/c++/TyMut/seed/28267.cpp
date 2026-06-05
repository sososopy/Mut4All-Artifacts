
template <typename> void bar ()
{
  [] <int> {}.operator () <> ();
}
void foo ()
{
  bar<int> ();
}


