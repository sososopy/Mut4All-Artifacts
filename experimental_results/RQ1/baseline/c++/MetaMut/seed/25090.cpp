
struct A
{
  virtual ~A () {}
};

struct B : public A
{
  virtual ~B () {}
};

template < int >
void test ()
{
  B *b = new B;
  b->~A ();
} 

int main ()
{
  test < 0 > ();
  return 0; 
}
