
struct S { int a, b[1]; };

constexpr S *p1 = (S*)0;
constexpr int *q1 = p1->b;


