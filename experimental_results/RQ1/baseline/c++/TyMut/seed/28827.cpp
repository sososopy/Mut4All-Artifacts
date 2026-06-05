
template<typename... T>
concept True = true;

static_assert(True<>);
