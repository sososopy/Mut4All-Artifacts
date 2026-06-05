
struct A;
void do_not_remove();
void f() { alignas(do_not_remove(A)); }
