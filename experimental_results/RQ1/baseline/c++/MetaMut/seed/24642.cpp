

struct A { };
struct B { };

class Test
{
public:
  Test()
  try
  {
    throw A();
  }
  catch(const A&)
  {
    try
    {
      throw B();
    } catch(const B&) {
    }
  }
};

int
main()
{
  try
  {
    Test x;
  }
  catch(const A&)
  {
  }
}

