
struct C {
   int a, b, c, d;
   void foo (void) {
     #pragma omp target private (a) firstprivate(b)
      { a = 1; b = 2; c = 3; this->d = 4; }
   }
};
int main() { C c; c.foo(); }
