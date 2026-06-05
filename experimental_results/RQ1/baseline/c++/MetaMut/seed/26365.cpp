
struct S
{
int m;
S(int i = 0) : m(i) { counter++;  }
};

volatile S s02 = 1;
const volatile S s03 = 1;

