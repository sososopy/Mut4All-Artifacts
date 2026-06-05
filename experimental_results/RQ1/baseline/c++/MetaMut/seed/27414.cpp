int main()
{
    struct S { int m; };
    return sizeof(char[S::m]);
}
