
struct X { 
  int i; 
  void clear() { i = 0; } 
}; 

void f(X* x) { 
  x->clear(); 
  x->~X(); 
} 

void g(X* x) {
  x->clear();
  delete x;
}

