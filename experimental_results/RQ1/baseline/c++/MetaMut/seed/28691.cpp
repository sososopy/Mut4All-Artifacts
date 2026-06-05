
template <class>
void
foo ()
{
  [](auto d) { if constexpr (auto [a, b]{d}; a) ; } (0);
}
template void foo<int> ();
