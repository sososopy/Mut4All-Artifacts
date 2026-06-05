
int i;
struct A { int n, a[]; } a = i ? A({ 1, { 2 } }) : A({ 2, { 3, 4 } });
int j;

int main ()
{
    __builtin_printf ("i = %i, j = %i, a = { %i, { ", i, j, a.n);
    for (int i = 0; i != a.n; ++i)
        __builtin_printf ("%i, ", a.a[i]);
    __builtin_printf ("} }\n");
}
