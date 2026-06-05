

    auto f = []{ return 0; };
    static_assert(requires { f(); });

