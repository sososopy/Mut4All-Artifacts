
struct A {
   A() { }
};

struct V {
   V() { };
   ~V() { }
};


struct S {
   S();
   ~S() {}

   union {
     A a = {};
     V v;
   };
};

S::S() = default;


