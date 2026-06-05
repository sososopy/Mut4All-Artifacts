
enum class A { A1, A2 };
A foo ();
long foo (int);

template <typename>
void
bar ()
{
  const auto c{foo ()};
}
