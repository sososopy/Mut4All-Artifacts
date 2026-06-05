template<typename> struct Trait { static constexpr bool value = false; };

template<typename T> concept Concept = Trait<T>::value;

static_assert( Concept<decltype(nullptr)> );


