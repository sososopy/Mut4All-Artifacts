
struct S { int a, b[1]; } s;

constexpr S *p0 = &s;
constexpr int *q0 = p0->b;

constexpr bool b0 = p0 == 0;
constexpr bool b1 = q0 == 0;

