
    template<typename Arg>
    auto & C(Arg && arg) { return arg; }

    void DecltypeWithinLambda()
    {
        int i = 0;
        [[maybe_unused]] auto l = [i]() // mutable
        {
            using T = decltype(C(i));
            [[maybe_unused]] T t(C(i)); // actual use of C() only matches decltype if mutable
        };
    }

