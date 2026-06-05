
class base {
protected:
    typedef void (base::*foo_type)() const;
    void foo() const {}
};

template <typename T>
struct bar : public base {
    foo_type test() { 
        return &base::foo;       // OK
    }
};

template <>
struct bar<void> : public base {
    using base::foo;
    foo_type test() { 
        foo();                   // OK
        base::foo();             // OK
        foo_type x = &bar::foo;  // OK
        return &base::foo;       // error
    }
};

int main() {
    bar<int>().test();
    bar<void>().test();
    return 0;
}
