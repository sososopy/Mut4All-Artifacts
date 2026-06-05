
template <typename String_>
struct Foo {
    explicit Foo(String_) {}
};


int main()
{
    const char*         bar = "bar";
    Foo<const char*&>   foo(bar);
}

