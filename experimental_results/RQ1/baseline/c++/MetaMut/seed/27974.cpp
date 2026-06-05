
struct A
{
    int data[2];
};


struct B : A
{
    using A::A;
};


int main()
{
    B b[2] = {{2,3,4}, {4,5,6, "azazezaz"}};
    return b[0].data[0];
}
