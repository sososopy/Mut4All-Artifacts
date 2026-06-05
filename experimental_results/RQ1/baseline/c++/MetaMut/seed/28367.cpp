#include <iostream>

unsigned Msb(uint64_t x) { return 63 - __builtin_clzl(x); }

template<typename T>
void f(T r, T p)
{
    if (p > r)
    {
        auto d = p - r;
        unsigned k = Msb(sizeof(T) == 4 ? unsigned(d) : d);
        std::cout << sizeof(T) << " k=" << k << "\n";
    }
}

int main()
{
  int data[] = {-1610499096,2086724600};
  f(data[0],data[1]);
}

