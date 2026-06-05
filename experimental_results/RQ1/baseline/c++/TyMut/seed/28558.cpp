
template<typename A, typename B>
struct unvariadic: private A, private B {
    // okay, provided A and B each declare appropriate
    // target_type and operator target_type() members
    using A::operator typename A::target_type,
          B::operator typename B::target_type;
};

template<typename... Bases>
struct variadic: private Bases... {
    // error: parameter packs not expanded with '...':
    using Bases::operator typename Bases::target_type...;
};

