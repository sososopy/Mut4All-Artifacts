
    struct S {}; // error occurs regardless of whether or not S is empty

    template<S s>
    struct T
    {
        T(); // declare ctor or mem fn
    };

    template<S s>
    inline T<s>::T() {} // define ctor or mem fn

