bool called = false;

struct Base {
  Base() { if (called) throw 1; called = true; }
};

struct B1 : virtual Base {
  B1() { }
};

struct C : B1, virtual Base {
  C() :
#ifdef FIX
    B1()
#else
    B1{}
#endif
  { }
};

int main() {
  C c;
}
