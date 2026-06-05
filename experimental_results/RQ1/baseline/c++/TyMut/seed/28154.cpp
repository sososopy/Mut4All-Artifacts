
struct S { constexpr S () : s (5) {}; int s; };
constexpr bool foo ()
{
  S *p = new S ();
  delete p;
  return true;
}
