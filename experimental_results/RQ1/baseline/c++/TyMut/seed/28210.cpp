
    struct foo {
        explicit foo(int) {}
    };

    struct bar {
        foo i;
    };

    bar x {
        .i{0} // <-- error:
    };

