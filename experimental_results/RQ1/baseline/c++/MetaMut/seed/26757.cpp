
struct foo {
    static void a() {}

    // fine
    auto b() const -> decltype( this->a() )
    {}

    // also fine
    template<typename X>
    auto c() -> decltype( this->a() )
    {}

    // even more fine
    template<typename X>
    auto d() const -> decltype( a() )
    {}

    // error: invalid use of incomplete type 'const struct foo'
    template<typename X>
    auto e() const -> decltype( this->a() )
    {}
};

int main()
{
}
