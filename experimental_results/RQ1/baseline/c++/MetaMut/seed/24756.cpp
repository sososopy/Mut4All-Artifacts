int foo(int *a, int b)
{
    return 0;
}

int main()
{
    int x;
    (struct {
          typeof(foo(&(struct { int t; }){.t = x}.t, 0)) t;
        }){0};
}