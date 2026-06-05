
struct A
{
    void delete_foo();
};

struct B : public A
{
    using A::delete_foo;
};

struct Foo { friend void B::delete_foo(); private: ~Foo() { } };

