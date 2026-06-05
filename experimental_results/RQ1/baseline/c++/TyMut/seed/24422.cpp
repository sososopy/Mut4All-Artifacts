

class C {
    struct T2 {
    };
    static void f() {}
};

template<typename T>
static inline void
f(T)
{
    // C::f(); // This generates an error correctly
    C::T2 __attribute__((unused)) v2; // This does not generate an error
}

int
main()
{
    // C::T2 v2; // This also generates an error correctly
    f(1);
    return 0;
}

