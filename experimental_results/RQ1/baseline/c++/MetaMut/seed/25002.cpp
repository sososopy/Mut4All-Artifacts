


typedef void T (int);

struct A {  operator T * ();  };
struct B {  operator T * ();  };
struct C : A, B {} c;

void f () {  c (0);  }
