
#if !defined(SCALE_FACTOR)
#define SCALE_FACTOR 23
#endif
template<int R, int N>
struct slow_fibonacci {
    static constexpr int value =
        slow_fibonacci<R, N-1>::value +
        slow_fibonacci<R + (1<<N), N-2>::value;
};
template<int R>
struct slow_fibonacci<R,2> {
    static constexpr int value = 1;
};
template<int R>
struct slow_fibonacci<R,1> {
    static constexpr int value = 1;
};
constexpr int x = slow_fibonacci<0,SCALE_FACTOR>::value;

