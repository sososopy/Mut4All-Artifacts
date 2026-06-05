

template < class > struct A {  int i;  };

struct B {  void i () {}  };

template < class T > struct C : A < T >, B
{ 
  void f () {  i ();  }
};

int main ()
{ 
  C < int > c;
  c.f();
  return 0;
}
