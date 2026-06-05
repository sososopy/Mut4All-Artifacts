
 struct A { virtual ~A(); };
 struct B: A {};
 bool f(A & a) { return dynamic_cast<B *>(&a) == nullptr; }

