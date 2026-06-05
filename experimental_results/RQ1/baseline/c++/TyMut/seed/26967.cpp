
class B
{
protected:
        B() { }
        B(int) { }
        void Foo() { }
        void Foo(int) { }
};
class D : public B
{
public:
        using B::B;
        using B::Foo;
};
void d_ctor()
{
        D d;
}
void d_ctor2()
{
        D d(0); // !
}
void d_foo(D* d)
{
        d->Foo();
}
void d_foo2(D* d)
{
        d->Foo(0);
}
