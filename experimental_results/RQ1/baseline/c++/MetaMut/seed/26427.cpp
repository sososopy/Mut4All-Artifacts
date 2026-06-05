

    #include <new>

    struct Foo {
        Foo() {}
        ~Foo() {}
    };

    struct X {
        Foo data[4];
    };

    template<class H>
    void f() {
        char storage[256];
        auto& object = *new(storage) X{};
        object.~X();
    }

