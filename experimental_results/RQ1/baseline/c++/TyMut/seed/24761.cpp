
template <unsigned> struct S
{
  S (int, int) { }
};

// template <> using S <-1> = int;

int main ()
{
  S <-1> i { 1, 2 };
}
