

    template <bool B>
    inline constexpr bool bool_ = B;

    #if defined(WORKAROUND)
    template<class T, class U>
    concept bool Same_impl = __is_same_as(T, U);
    #else
    template <class T, class U>
    concept bool Same_impl = bool_<__is_same_as(T, U)>;
    #endif

    template<class T, class U>
    concept bool Same = Same_impl<T, U> && Same_impl<U, T>;

    template<class T>
    concept bool Foo = Same<const T&, const T&>;

    template<class T>
    concept bool Bar = Foo<T> && Same<T, T>;

    template<class T>
    struct S1 {
        // overload set incorrectly is ambiguous (should resolve to second overload)
        static constexpr bool f() requires Foo<T> { return false; }
        static constexpr bool f() requires Bar<T> { return true; }
    };

    template<class T>
    struct S2 {
        // overload set incorrectly is not ambiguous (resolves to third overload)
        static constexpr bool f() requires Foo<T> { return false; }
        static constexpr bool f() requires Bar<T> { return false; }
        static constexpr bool f() requires bool_<true> && true { return true; }
    };

    template<class T>
    concept bool can_f = requires { T::f(); };

    int main() {
        static_assert(Foo<int>);
        static_assert(Bar<int>);

        static_assert(can_f<S1<int>>);  // Fails
        static_assert(S1<int>::f());    // Bogus error

        static_assert(!can_f<S2<int>>); // Fails
    #ifndef WORKAROUND
        static_assert(S2<int>::f());    // Bogus non-error
    #endif
    }

