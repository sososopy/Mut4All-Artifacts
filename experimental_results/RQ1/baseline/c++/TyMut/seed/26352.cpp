
struct Foo {
    Foo() = default;
    Foo(Foo const&) = delete;
    Foo(Foo&&) = delete;
};

Foo foo() { return {}; }

template<typename F>
struct call_wrapper {
    F&& f;

    constexpr operator decltype(auto)() && {
        return static_cast<F&&>(f)();
    }
};
template<typename F>
call_wrapper(F&&) -> call_wrapper<F>;

struct Bar : Foo {
    Bar() : Foo(call_wrapper{foo}) {}
};
