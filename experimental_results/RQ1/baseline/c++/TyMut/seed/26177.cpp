
struct S { S(int&); ~S(); };
void f() { int n = (S(n), 0); }

