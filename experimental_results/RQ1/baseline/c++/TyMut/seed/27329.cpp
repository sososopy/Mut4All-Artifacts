
class A
{
public:
    A()
    {
        printf("A create.\n");
    }
    A(const A&a)
    {
        printf("A copy create.\n");
    }
    A(A&&a)
    {
        printf("A move create.\n");
    }
    ~A()
    {
        printf("A delete.\n");
    }
};
A newA()
{
    A a;
    return a;
}
A&& bug(A&& x)
{
    printf("bug\n");
    return std::move(x);
}
int main()
{
    A &&a=newA();              //ok
    printf("----------\n");
    A &&b=bug(newA());         //error
    printf("----------\n");
    A c=bug(newA());           //ok
    printf("----------\n");
    return 0;
}

