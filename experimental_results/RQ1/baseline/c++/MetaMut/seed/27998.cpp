

struct A { int a : 4; };
struct B : A { int b : 3; };
static_assert(__is_standard_layout(A), "");
static_assert(!__is_standard_layout(B), "");

struct C { int : 0; };
struct D : C { int : 0; };
static_assert(__is_standard_layout(C), "");
static_assert(!__is_standard_layout(D), "");
