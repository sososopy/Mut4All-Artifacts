
struct Foo{};
struct Bar{};

struct Base {
    Foo func1(const Foo , const Bar = Bar{}) const {
        return {};
    };
};

struct Derived : public Base {
    using Base::func1;
    Foo func1(const Foo ) const {
        return {};
    };
};

int main() {
    Foo foo;
    Derived der;
    der.func1(foo);
}

