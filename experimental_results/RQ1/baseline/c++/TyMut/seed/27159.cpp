
struct A
{
    A();
    A(A&&);
    A(A const&);
    A& operator=(A&&);
    A& operator=(A const&);
};

A getA()
{
    {
        A a;
        return a;
    }
}

int main()
{
    const A a=getA();
}
