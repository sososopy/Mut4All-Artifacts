
struct a
{
  static int b;
};

template <typename>
void
foo ()
{
  &(a::b);
}
