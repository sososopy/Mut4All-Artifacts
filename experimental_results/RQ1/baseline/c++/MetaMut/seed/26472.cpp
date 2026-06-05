
template <double> struct S {};
template <typename Hash, int i> struct bucket {};
template <typename Hash, int i>
int find_indices_impl(bucket<Hash, i> const &);
using HashTable = bucket<S<2.0>, 1>;
auto t = find_indices_impl<S<1.0>>(HashTable{});
