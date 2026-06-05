
struct A
{
  int i();
};

struct B : public virtual A
{
  int get()
  {
    return A::i() + 1;
  }
};

struct C final : public B
{
  int get()
  {
    return A::i() + 2;
  }
};

int foo(C& c)
{  
  return c.get(); // Need not go via vtable pointer as class C is final
}

int foo(B& b2)
{
  return b2.get(); // This has to go via vtable as most derived class can change the location of A
}
