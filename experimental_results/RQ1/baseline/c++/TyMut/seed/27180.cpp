
extern "C" int printf(const char *, ...);

struct Bar {
    Bar(int n) {
        printf("Bar(%d)\n", n);
        if (n > 0)
            throw 2;
    }

    ~Bar() { printf("~Bar\n"); }
};

struct Foo {
    Bar b1 = 0;
    Bar b2 = 1;
    Foo() { }

    ~Foo() { printf("~Foo()"); }
};

int
main()
{
    try {
        Foo f;
    } catch(int) {
        printf("catch\n");
    }
}

