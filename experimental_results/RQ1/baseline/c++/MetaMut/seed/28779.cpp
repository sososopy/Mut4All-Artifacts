
template <typename Function, typename T>
concept Callable1Arg = requires(Function func, T value) {
    func(value);
};

// Should fail and fails:
static_assert(Callable1Arg<int(*)(int*), bool>);


