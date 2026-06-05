
#include <type_traits>

class S {
	int i;
};

struct P {
	char c;
	S s;
} __attribute__ ((packed));

static_assert(std::is_pod<S>::value, "S should be a POD type");
static_assert(sizeof(P) == sizeof(char) + sizeof(S), "P should be packed");

