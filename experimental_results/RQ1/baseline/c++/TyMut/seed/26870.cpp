
template<typename, typename> struct C { int n; };
struct D { int k = C<D, D>().n; };

