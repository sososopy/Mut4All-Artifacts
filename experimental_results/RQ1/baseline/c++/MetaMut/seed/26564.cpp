
    struct A
    {
        A() {}
        int x;
    };

    struct B : A {};

    int main()
    {
        B b = B();
        return b.x;
    }

