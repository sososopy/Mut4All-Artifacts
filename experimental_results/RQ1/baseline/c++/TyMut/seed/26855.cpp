

typedef int int_t;

template <int_t &>
struct S { };

int_t a;
S <a> b;
