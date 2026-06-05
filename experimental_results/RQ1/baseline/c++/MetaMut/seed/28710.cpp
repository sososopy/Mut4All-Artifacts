
#include <type_traits>

template <typename T>
struct TmpArray
{
   T arr[1];
};

template <typename Src, typename Dst, typename = void>
struct is_non_narrowing_conversion : std::false_type
{};

template <typename Src, typename Dst>
struct is_non_narrowing_conversion<
    Src, Dst,
    decltype(void(TmpArray<Dst>{{ std::declval<Src>() }}))
> : std::true_type
{};

struct mystruct
{
    int a;
    void * b;
};


void test_nok()
{
    is_non_narrowing_conversion<int&, mystruct>::type v;
}

