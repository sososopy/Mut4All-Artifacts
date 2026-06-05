
    #include <type_traits>

    void func_with_int_arg(int) {}

    template<typename T>
    std::enable_if_t<std::is_same<T, int*>::value>
    func(T arg)
    {
        func_with_int_arg(static_cast<int*>(arg));  // (*1)
    }

    template<typename T>
    std::enable_if_t<std::is_same<T, int>::value>
    func(T) {}

    int main(int, char **)
    {
        func(1);  // Instantiate func<int>
    }
