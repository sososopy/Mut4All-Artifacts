

typedef __typeof__(sizeof(int)) size_t;
template <size_t n>
struct is_array_of {};

template <typename MemTag, typename... Args>
typename is_array_of<sizeof...(Args)>::value
coords(Args... args) {
    auto c1 = coords<int>(0ul, 0ul, 0ul);
}
