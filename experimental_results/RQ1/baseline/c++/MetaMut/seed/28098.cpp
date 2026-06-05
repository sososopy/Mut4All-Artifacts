
#include <array>
#include <map>

template <template <class ...> class ttp, class c>
struct resolve {static constexpr bool value = false; };

template <template<class ...> class ttp, class ... ts>
struct resolve<ttp, ttp<ts...>>{static constexpr bool value = true; };

template < class c, template <class ...> class ttp>
concept instanceof = resolve<ttp, c>::value;

int main()
{
    instanceof<std::map> auto x = std::array<int, 3>{1, 2, 3};
}

