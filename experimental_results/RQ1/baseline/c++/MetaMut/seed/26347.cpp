
template <typename Derived>
struct base {
    friend void foo(const Derived& d)
        requires requires { bar(d); } // removing this makes it work
    {
        bar(d);
    }
};

namespace adl {
struct S : base<S> {
    friend void bar(const S&) {}
};
}  // namespace adl

void test(adl::S const& s) {
    foo(s);  // error: 'foo' was not declared in this scope
}
