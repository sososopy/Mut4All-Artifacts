
struct A {
    const char *a;
    const char *b[];
};

struct B: A {
    const char *c;
};

extern "C" {
    void free (void*);
    void* malloc (__SIZE_TYPE__);
    int printf (const char*, ...);
}

void foo (B *b)
{
    const char* const s = "";

    b->c = s;

    b->b [0] = 0;

    if (b->c != s)
        __builtin_abort (); 
}

int main ()
{
    B *b = (B*)malloc (sizeof *b + 2 * sizeof *b->b);
    b->a = "a";
    b->c = "c";
    b->b [0] = "b[0]";
    b->b [1] = "b[1]";
    b->b [2] = "b[2]";
    b->b [3] = "b[3]";

    printf ("{ { a = %s, b = { %s, %s, %s, %s } }, c = %s }\n",
            b->a, b->b[0], b->b[1], b->b[2], b->b[3], b->c);

    foo (b);

    free (b);
}
