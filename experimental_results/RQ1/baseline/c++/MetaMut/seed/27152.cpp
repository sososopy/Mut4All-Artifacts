
    struct nontrivial {
        nontrivial();
        nontrivial(nontrivial const&);
        long a;
    };

    struct foo {
        nontrivial nt;
        char b;
    };

    struct bar : foo {};

    foo get_foo();

    bar a{
        get_foo()
    };


