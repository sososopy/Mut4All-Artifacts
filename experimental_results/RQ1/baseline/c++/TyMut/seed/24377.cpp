struct A
{
    const int b;
};

union U
{
    A a;
};

int main()
{
    U a = {1, 1};
}

