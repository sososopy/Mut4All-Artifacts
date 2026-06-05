
#include <compare>

class C
{
public:
	int a;
	constexpr bool operator<=>(C const& other) const = default;
};

// auto b = C{17} <=> C{23};
