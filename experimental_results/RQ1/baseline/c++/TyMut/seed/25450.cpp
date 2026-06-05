

struct A
{ 
  virtual ~A () {}
};

struct B : public A
{ 
  virtual ~B () {}
};

template < int > void foo ()
{ 
  B *b = new B;
  b->~A ();
}

int main ()
{ 
  foo < 0 > ();
  return 0;
}
