
template<typename T>
concept Concept = requires (T x) {
    {[](){ return T::foo; }()};
};
static_assert(!Concept<int>);


