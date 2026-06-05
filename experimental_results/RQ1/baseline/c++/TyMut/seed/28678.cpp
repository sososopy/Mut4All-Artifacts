
template <class T>
int f ()
{
  int i = { 2.2 };   // missing -Wnarrowing
  return i;
}

template <class T>
int g ()
{
  int i = { 2.2 };   // missing -Wnarrowing despite explicit instantiation
  return i;
}

template <int> int g ();

