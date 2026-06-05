
struct A
{ 
  // Should be: A & operator= (const A &);
  // or: A & operator= (A);
  // or: A & operator= (A &&);
  A & operator= (A &);
};

struct B : A {};

void foo ()
{ 
  B b;
  b = B ();
}
