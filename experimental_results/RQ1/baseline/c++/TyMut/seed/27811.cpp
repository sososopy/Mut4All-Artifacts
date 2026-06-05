
void
foo ()
{
  auto l = [](int){ return 42; };
  static_assert(noexcept((int (*)(int))(l)), "");
}
