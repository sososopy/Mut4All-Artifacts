
template<typename TSomeTemplate>
struct A
{
    static constexpr std::size_t B = 1;

    template<typename T, typename... Ts>
    static constexpr std::size_t C = 4;

    template<typename T>
    static constexpr std::size_t C<T> = B; //rejected by msvc and gcc
};

int main()
{
    int i = A<int>::C<int>;

    return 0;
}
