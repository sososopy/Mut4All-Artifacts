
struct A { A(int); A() = delete; };
struct B { B(int); B() = delete; };
struct C : B { using B::B; A a = 0; } c(0);

