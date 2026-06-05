
template<typename Ts>
struct refines
  : Ts
{};

struct A
{};

struct B
  : refines<A>
{};

struct C
  : refines<B>
{};

void fun(void *)
{}

template<typename T>
int fun(refines<T> *)
{
  return 0;
}

int main()
{
  C *p = 0;
  int i = fun(p);
}
