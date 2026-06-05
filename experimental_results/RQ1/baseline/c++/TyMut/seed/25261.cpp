

struct A
{
  A ();
};
template <typename T>
struct C
{
  C (int, const T &, const A & = A ());
};

C a = { 0, 0 };
