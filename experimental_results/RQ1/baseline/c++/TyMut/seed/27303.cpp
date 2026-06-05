
enum E : int { F };
struct X {
  E e{F};
};
constexpr X x[1];

auto
foo ()
{
  return x[0].e;
}
