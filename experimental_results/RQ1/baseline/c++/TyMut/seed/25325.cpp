
struct A {
  struct {
    int j = this;
  } b;
  int a = b.j;
};

void f(A) { 
  f(A{}); 
}
