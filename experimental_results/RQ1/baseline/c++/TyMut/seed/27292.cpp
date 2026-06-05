
    template <class, class...>
    concept always_satisfied = true;

    using arg_alias = int;

    template <always_satisfied F>
    using result_of = decltype(F{}(arg_alias{}));

    template <class F>
    always_satisfied<result_of<F>> auto foo(F) {}

    void bar() { foo([](auto){}); }

