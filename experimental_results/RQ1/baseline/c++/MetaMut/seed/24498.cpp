
void f (__fp16 *x) { }

void g (__fp16 *x, __fp16 *y) { }

template <typename T, typename U> struct S { static int i; }; 
template <> int S<__fp16, __fp16>::i = 3;

