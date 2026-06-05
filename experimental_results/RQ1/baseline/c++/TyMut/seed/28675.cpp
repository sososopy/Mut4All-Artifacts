
struct S {} s;
template <S> concept C = requires { [a] = s; };
static_assert(C<s>);

