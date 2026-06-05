
#define BUG

template <typename T, unsigned int N>
constexpr T sum(const T (&arr)[N])
{
    T result = 0;
#ifndef BUG
    for (unsigned i = 0; i < 5; ++i)
      result += arr[i];
#else
    for (T i : arr)
        result += i;
#endif
    return result;
}

int main()
{
    constexpr int ARR[] = { 1, 2, 3, 4, 5 };
    static_assert(sum(ARR) == 15, "");
}

