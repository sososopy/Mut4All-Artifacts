
    template<typename T>
    using ref = T&;

    template<typename T, typename C>
    using mem = T C::;


    #include <cstdio>
    #include <type_traits>

    struct S {
        ref<mem<int,S>> x;
    };

    int fun1() {
        std::printf("%i\n", (int)sizeof(S));

        std::aligned_storage<S>::type X;
    }


    struct S {};

    mem<int, S> foo = 1;

    int S::foo2 = 1; // error

    int fun2() {
        mem<int, S> foo3 = 3;

        int S::foo4 = 4; // error

        return foo3;
    }


    int fun3() {
        mem<int, S> foo;
        int S::*foo2 = &foo; // error
    }


