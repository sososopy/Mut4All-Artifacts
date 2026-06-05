
template <typename T1, typename S1>
struct B { };

template <typename T1>
struct A
{
    private:
    template <typename T2, typename S2>
        static B<T2, S2> bar();

    public:
    template <typename S2>
        auto foo1() -> decltype(bar<T1, S2>());
};

// (*)
template<>
template<>
auto A<int>::foo1<float>() -> B<int, float>;
