
template <class Ub> concept A42b = true;
template <class Tc> concept R42c = A42b<Tc&>;

static_assert (R42c<void>);


