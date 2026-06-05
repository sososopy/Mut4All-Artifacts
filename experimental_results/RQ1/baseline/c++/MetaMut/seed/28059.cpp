struct S { int a[2]; };

template <typename T>
void
fn ()
{
  constexpr S s = S();
}

void
foo ()
{
  fn<int>();
}

