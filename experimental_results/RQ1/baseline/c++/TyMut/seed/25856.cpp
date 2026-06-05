
    struct S
    {};

    template<typename X, typename Y>
    struct T
    {
        template<typename A>
        T(X x, A &&a)
        {}

        template<typename A>
        explicit T(A &&a)
            noexcept(noexcept(
                T(X(), static_cast<A &&>(a))
            ))
          : T(X(), static_cast<A &&>(a))
        {}
    };

    template<typename X, typename Y>
    struct U
      : T<X, Y>
    {
        using T<X, Y>::T;
    };

    // Comment this function out and everything works
    U<S, char> foo(char ch)
    {
        return U<S, char>(ch);
    }

    int main()
    {
        U<S, int>  a(42);   // This works
        U<S, char> b('4');  // This fails
    }
