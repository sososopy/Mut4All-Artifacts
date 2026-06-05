
struct a
{
	union {int i;};
};

template <int (a::*p)>
struct b : a
{
	b () {this->*p = 0;}
};

b<&a::i> c;
