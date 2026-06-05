
#include <tuple>

template <unsigned N>
struct s
{
	template <typename R>
	struct ss
	{
		R _1;
		unsigned _2;
	};

	constexpr auto f ()
	{
		return ss { std::pair <char, int> { 'a', 1 }, 1 };
	};
};

