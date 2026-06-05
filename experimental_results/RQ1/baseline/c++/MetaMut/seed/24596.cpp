
struct A { explicit A(int = 0); };
struct B { A a; };

int main()
{
  B b = {};
}
