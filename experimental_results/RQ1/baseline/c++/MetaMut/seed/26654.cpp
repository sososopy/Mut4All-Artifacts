
struct A { int i; };

struct B: virtual A { };

int main ()
{
    return __builtin_offsetof (B, i);
}
