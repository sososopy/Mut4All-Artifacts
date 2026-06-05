

    #include <type_traits>
    #include <utility>
    #include <tuple>
    
    #define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
    
    struct sinker
    {
        template <typename T>
        void sink(T&)
        {
        }
    };
    
    template <typename T, typename TF>
    void caller(T& v, TF&& f)
    {
        sinker s;
        f(s, v);
    }
    
    template <typename T>
    void interface(T& v)
    {
        return caller(v, [](auto& xs, auto&& xv) -> decltype(auto)
            {
                xs.sink(FWD(xv));
            });
    }
    
    int main()
    {
        int x = 0;
        interface(x);
    }

