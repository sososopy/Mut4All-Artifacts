
struct A
{
    struct B
    {
        int i = 0;
    };

    B b;

    A(const B& _b = B())
        : b(_b)
    {}
};

