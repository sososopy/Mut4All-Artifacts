
struct a
{
	union {int i;};
};

template <int (a::*)> struct b;

typedef b<&a::i> c;
