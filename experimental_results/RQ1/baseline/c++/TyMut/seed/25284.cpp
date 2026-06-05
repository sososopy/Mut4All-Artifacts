

    #include <type_traits>
    template<typename T>
    struct foo
    {
        using sub = struct some_struct;
    };
    
    template<typename T>
    struct bar
    {
        using sub = struct some_struct { };
    };
    
    int main()
    {
        static_assert(std::is_same<foo<int>::sub, foo<double>::sub>::value, "should not pass");
        static_assert(std::is_same<bar<int>::sub, bar<double>::sub>::value, "should not pass");
    }

