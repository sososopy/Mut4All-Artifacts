
#include <memory>
#include <random>

struct X
{
    X () : m_n (std::unique_ptr<int> (new int)) { if (random () & 1) throw 1; }
    std::unique_ptr<int> m_n;
};

void foo (std::initializer_list<X>)
{
}

int main ()
{
  for (int i = 0; i < 10; ++i)
  {
    try
    {
      foo ({ X{}, X{} });
    }
    catch (...) {}
  }
}
