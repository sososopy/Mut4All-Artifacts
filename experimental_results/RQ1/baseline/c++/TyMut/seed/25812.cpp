
    struct A{
        A(int);
    };
    struct B:A{
        using A::A;
    };

    B x{42};

