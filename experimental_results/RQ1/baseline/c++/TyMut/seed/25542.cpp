
    struct S
    {
        template<class A, class...B>
        S(A, B...) {}
    };

    struct T : S
    {
        using S::S;
    };

    struct U : T
    {
        using T::T;
    };

    int main()
    {
        U u{"hello"};
    }
