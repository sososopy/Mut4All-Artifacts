template <class>
concept C = []<auto>{}.operator()<>;

static_assert(C<int>);

