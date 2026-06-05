
template <typename C>
class S {
public:
    void add (C c) { ++cnt; }
    size_t size () const { return cnt; }

private:
    size_t cnt {}; 
};

struct Foo1 {};
struct Foo2 {};
struct Foo3 {};

class Z : public S<Foo1>, public S<Foo2>, public S<Foo3> {
public:
    using S<Foo1>::add;
    using S<Foo2>::add;
    using S<Foo3>::add;

    using S<Foo1>::size;    // (1)
    using S<Foo2>::size;    // (2)
    using S<Foo3>::size;    // (3)
};


Z z;

z.add (Foo1 {});
z.add (Foo1 {});
z.add (Foo2 {});

cout << z.size () << endl;

