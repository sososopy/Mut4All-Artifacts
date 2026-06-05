
class A{
    private:
        struct B{} _b;
};

template <typename T> class C{
    public:
        A::B* _b;
};

C<void> c;
