
template<int ...a> constexpr auto x = [...z = a] (auto F) { return F(z...); };

