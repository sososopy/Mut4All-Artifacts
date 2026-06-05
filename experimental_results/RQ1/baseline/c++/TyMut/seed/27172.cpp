
struct A { constexpr A(); };
void f ()
{
  A b[2][3];
  [b] {};
}


