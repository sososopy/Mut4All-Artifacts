namespace A {
    template<class>
    class B {
        template<class> friend class C;
    };
}

void foo()
{
    A::B<void> b;
}

namespace A {
    template<class>
    class C {
    };
}

using namespace A;

void bar()
{
    C<void> c;
}

