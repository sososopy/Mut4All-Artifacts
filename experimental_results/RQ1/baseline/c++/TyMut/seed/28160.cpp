
    #include <type_traits>

    template <typename Signature>
    struct declfunc;

    template <typename Result, typename... Args>
    struct declfunc<Result(Args...)>
    {
        Result operator() (Args...);
    };

    template <typename... Signatures>
    struct decloverload
        : declfunc<Signatures>...
    { };

    using overload_set = decloverload<void(int), void(int, int)>;
    static_assert(std::is_invocable_v<overload_set, int>);

