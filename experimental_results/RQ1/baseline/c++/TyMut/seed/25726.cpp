
enum A : enum { };

template<typename> struct B {};

B<__underlying_type(A)> b;
