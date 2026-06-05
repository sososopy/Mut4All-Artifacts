
 struct S0 { virtual void f0() = 0; };
 struct S1: S0 { void f0() { S0::f0(); } };
 struct T {
     T(S1 * p) { p->f0(); }
     T(T const &);
 };
 struct S2: S1 {
     virtual T f1(bool b);
     virtual T f2(bool b);
 };
 T S2::f1(bool b) {
     if (b) return this;
     return this;
 }
 T S2::f2(bool b) {
     if (b) return this;
     return this;
 }

