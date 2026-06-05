
    #include <tuple>

    template <size_t N, typename T, typename... Args>
    struct DimTupleImpl {
        using type = typename DimTupleImpl<N-1, T, Args..., T>::type;
    };

    template <typename T, typename... Args>
    struct DimTupleImpl<0, T, Args...> {
        using type = std::tuple<Args...>;
    };

    // Tuple with N elements all of type T
    template <size_t N, typename T>
    struct DimTuple {
        using type = typename DimTupleImpl<N, T>::type;
    };

    template <typename... Params>
    void foo(Params... params) {
        auto tuple = std::make_tuple((size_t)params...);
    }

    template <typename... Params>
    void bar(Params... params) {
        typename DimTuple<sizeof...(Params), size_t>::type tuple = std::make_tuple((size_t)params...);
    }

    int main() {
        foo(1,2,3); // Fails, clang++ compiles it
        bar(1,2,3); // Compiles correctly
        return 0;
    }

