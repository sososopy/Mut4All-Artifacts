<typename T> struct S { typename T::error type; };
struct X {};
void f(S<int> *);
void f(...);
void g() { f((X*)0); }

