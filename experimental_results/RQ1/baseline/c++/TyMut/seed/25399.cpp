

struct A
{ 
  static int x;
};

void f ()
{ 
  using ::A;
  x;
}
