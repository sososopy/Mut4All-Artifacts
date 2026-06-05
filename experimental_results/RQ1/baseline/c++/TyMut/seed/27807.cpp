
inline int inline1 = 0;
inline int inline2 = 0;
static_assert(&inline1 == &inline1);
static_assert(&inline1 != &inline2);

But this code doesn't:

template<typename T>
inline int NullT = 0;

struct A1;
struct A2;

static_assert(&NullT<A1> == &NullT<A1>);
static_assert(&NullT<A1> != &NullT<A2>);

