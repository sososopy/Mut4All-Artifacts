
class Foo
{
public:
    static bool init();
};

static bool FooInit = Foo::init();

int main()
{
    return 0;
}

