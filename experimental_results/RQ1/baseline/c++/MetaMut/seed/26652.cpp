template <int... x> concept bool A_concept = (... || (x < 0));
template <int... x> constexpr bool A_constexpr = (... || (x < 0));

static_assert(A_concept<-1, 1>);   // This assertion fails, while it should not
static_assert(A_constexpr<-1, 1>); // OK.

