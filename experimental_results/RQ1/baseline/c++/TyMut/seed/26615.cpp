
extern "C" int printf (const char*, ...);

struct __attribute__ ((aligned (32))) X { char a [32]; };

int main ()
{
    X *p = new X ();
    if (reinterpret_cast<unsigned long>(p) & (alignof (X) - 1))
        printf ("%p not %lu-byte aligned\n", p, alignof (X));
}
