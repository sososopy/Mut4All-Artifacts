
#include <type_traits>

struct ZZZ
{
    template <typename Func>
    auto foo(Func func) -> std::result_of_t<Func(int*)>
    {
        return func(static_cast<int*>(nullptr));
    }

    template <typename Func>
    auto foo(Func func) const -> std::result_of_t<Func(const int*)>
    {
        return func(static_cast<const int*>(nullptr));
    }
};

int main()
{
    const ZZZ zzz;

    zzz.foo(
        [&](auto* pointer)
            // specifying the return type explicitly will fix the issue
            //-> void
        {
            static_assert(std::is_same_v<decltype(pointer), const int*>, "");
        });
}
