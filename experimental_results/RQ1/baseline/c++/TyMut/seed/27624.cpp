

template<typename T>
struct S{};

struct A
{
  virtual void func() = 0;
};

int main()
{
  S<A(int)> s;
}

