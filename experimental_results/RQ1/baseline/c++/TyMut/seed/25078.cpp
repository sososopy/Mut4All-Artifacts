

class Foo {
public:
        explicit Foo(bool)
        {}
};

class Bar {
public:
        Bar()
        : foo_(new Foo(this))
        {}
private:
        Foo * foo_; 
};

int main()
{
        Bar bar;
        return 0;
}


