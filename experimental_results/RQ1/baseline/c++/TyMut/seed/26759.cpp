
class Blah1
{
public:
    Blah1() {}
    virtual ~Blah1() {}

    virtual int foo() { return 0; }
};

class Blah2 : public Blah1
{
public:
    Blah2() {}
    ~Blah2() {}

    int boo() { return 1; }
    int foo() override { return 1; }
};

Blah2 g_blah2;

int do_something()
{
    Blah2 *p_blah2 = &g_blah2;
    int i = p_blah2->foo();      // <----- crash here
}

