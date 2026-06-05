
struct Foo
{};

struct ConvertibleToFoo
{
    operator const Foo&() const &;

    operator Foo&&() &&;
};

void bar()
{
    Foo f1(static_cast<const ConvertibleToFoo&>(ConvertibleToFoo{}));
    Foo f2(static_cast<ConvertibleToFoo&&>(ConvertibleToFoo{}));
    f1 = static_cast<const ConvertibleToFoo&>(ConvertibleToFoo{});
    f2 = static_cast<ConvertibleToFoo&&>(ConvertibleToFoo{});
}
