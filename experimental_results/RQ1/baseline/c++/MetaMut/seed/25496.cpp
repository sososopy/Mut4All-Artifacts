
class A { 
  virtual void f();
};

class B : virtual A { 
  virtual void f();
};

class C : virtual B {}; 

extern "C" int printf(const char *fmt, ...);

int main() {
  printf("sizeof(C) = %lu\n", sizeof(C));
}
