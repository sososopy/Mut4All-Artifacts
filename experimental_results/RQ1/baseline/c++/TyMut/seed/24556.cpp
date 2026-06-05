
struct Foo
{
  struct A
  {
    const int &container;
    const int &args;
  };
  static void Create (const A &);
};

int main ()
{
  Foo::Create ({{}, {}});
}


