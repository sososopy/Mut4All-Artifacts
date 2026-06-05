
struct foo {
    void bar() & {}
    void bar() && {}
};

int main()
{
    // No complaints
    auto p = &foo::bar;
    // error: pointer-to-member-function type 'void (foo::*)() &' requires an lvalue
    (foo{}.*p)();
}

