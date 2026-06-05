
int main()
{
  constexpr const int &r = 42;
  static_assert(r == 42); // { dg-bogus "" }
}
