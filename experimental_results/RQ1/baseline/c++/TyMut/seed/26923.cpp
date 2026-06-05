
struct A { int i; };
struct B: virtual A { };

__SIZE_TYPE__ a[] = {
    !&((B*)0)->A::i,
    __builtin_offsetof (B, A::i)
};

int main ()
{
    return a[0] + a[1];
}
