
struct S { };
void f ()
{
  auto a = reinterpret_cast<S&&>(f());
}

