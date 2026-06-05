
struct S {};
struct T : S {};
void foo(T *t) { t->~S(); }

