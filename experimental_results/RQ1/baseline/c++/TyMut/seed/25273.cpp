
void __attribute__ ((noclone, noinline)) f (void *p) { }

void __attribute__ ((noclone, noinline)) g (int n)
{
    char a [n] = "";
    f (a);
}

int main ()
{
    g (N);
}

