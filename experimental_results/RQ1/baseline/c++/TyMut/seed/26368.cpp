
struct S
{
  static int operator[] (int x) { int a[2]; return a[x]; }
};
template<class T> int f ()
{
  S s;
  s[0]++;
  return 0;
}

