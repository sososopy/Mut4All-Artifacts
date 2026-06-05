
template<typename... Args>
auto make_vector(const Args&... elems){
    return std::vector{elems...};
}
auto v2 = make_vector(std::vector{1,2,3});
static_assert(std::is_same_v<decltype(v2), std::vector<int>>);
static_assert(std::is_same_v<decltype(v2), std::vector<std::vector<int>>>);
