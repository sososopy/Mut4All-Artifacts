

struct A {
  friend void f(int=0);   // (A), ill-formed, [dcl.fct.default]p4
  friend void g(int=0){}; // ( ), legal
};

void g(int);              // (B), ill-formed, [dcl.fct.default]p4

int main () { }

