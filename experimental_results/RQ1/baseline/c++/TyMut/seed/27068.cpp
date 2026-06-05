

    class A
    {
    public:
       A(const A&) : a(a), b(b), c(c) {}
    private:
       int a;
       int* b;
       int& c;
    };

