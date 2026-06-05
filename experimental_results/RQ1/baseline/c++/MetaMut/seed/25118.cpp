
template <class U>
struct A
{
  A() {}
};

struct B
{
  A<void()> a;
};

void func(B b = {}) {}

int main()
{
  func();
}

