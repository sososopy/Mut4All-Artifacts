
    struct base {
        consteval base() {}
    };

    struct child : base {
        using base::base;
    };

    constexpr child i;

