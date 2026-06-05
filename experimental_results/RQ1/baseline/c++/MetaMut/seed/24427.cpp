
#include <type_traits>
template<typename T, typename A1, typename... Args>
struct CountType
{
	static constexpr int value = CountType<T, A1>::value + CountType<T, Args...>::value;

	constexpr operator int() const
	{
		return value;
	}
};

template<typename T, typename A1>
struct CountType<T, A1>
{
//	static constexpr int value = static_cast<int>(std::is_same<T, A1>());  // this works
	static constexpr int value = static_cast<int>(std::is_same<T, A1>{});

	constexpr operator int() const
	{
		return value;
	}
};


template<typename T, typename... Args>
using CountDecayType = CountType<T, std::decay_t<Args>...>;


template<typename... Args>
void foo(Args&&... args)
{
	int i = CountDecayType<int, Args...>{};
}


int main()
{
    foo(5, 6L, 5, 5);

    return 0;
}


