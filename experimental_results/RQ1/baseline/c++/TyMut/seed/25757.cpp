
#pragma GCC visibility push(default)

namespace std
{

  template<class _E>
    class initializer_list
    {
    public:
      typedef _E value_type;
    };

}

#pragma GCC visibility pop

struct A
{
  int i;
  A(std::initializer_list<int>) { }
};

int x = 4;
int main(int argc, char **argv)
{
  { int i[x] = { 42, 42, 42, 42 }; }
  {
    A a[x] = { argc };
    if (a[1].i != 42)
      __builtin_abort ();
  }
}
