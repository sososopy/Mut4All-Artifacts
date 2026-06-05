
template <class T1, class T2>
struct compressed_pair
{
  T1 t1 [[gnu::empty]];
  T2 t2 [[gnu::empty]];
// ... all the member functions
};

struct Empty { };
static_assert (sizeof (compressed_pair<int, Empty>) == sizeof (int), "too long");


