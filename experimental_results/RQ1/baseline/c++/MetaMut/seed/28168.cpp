
struct A
{
  A (int);
};

struct BB
{
  A v;
};

struct B : BB
{};

B
foo ()
{
  return {{B {{42}}}};
}
