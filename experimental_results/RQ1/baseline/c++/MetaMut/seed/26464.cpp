
    export module A;

    export struct A
    {
        static constexpr int blub = -1;
    };


    import A;

    int main()
    {
        const int& x = A::blub;
    }

