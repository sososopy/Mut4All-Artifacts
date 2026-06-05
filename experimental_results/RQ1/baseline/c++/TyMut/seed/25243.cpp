
enum { X = 12345678, Y = 87654321 };

struct A { int n, a []; };
struct A a0 = { 1, { 2, 3, 4 } };

void print (const int *i, struct A *a, const int *j)
{
    __builtin_printf ("\ni = %i\n"
                      "a = { %i, { %i, %i, %i }\n"
                      "j = %i\n",
                      *i, a->n, a->a[0], a->a[1], a->a[2],
                      *j);

    if (*i != X || *j != Y || a->a[0] != 2 || a->a[1] != 3 || a->a[2] != 4)
       __builtin_abort ();
 }

int main ()
{
    {
        static int i = X;
        static struct A a = { 1, { 2, 3, 4 } };
        static int j = Y;

        print (&i, &a, &j);
    }
    {
        __thread int i = X;
        __thread struct A a = { 1, { 2, 3, 4 } };
        __thread int j = Y;

        print (&i, &a, &j);
    }
    {
        int i = X;
        struct A a = { 1, { 2, 3, 4 } };
        int j = Y;

        print (&i, &a, &j);
    }
}
