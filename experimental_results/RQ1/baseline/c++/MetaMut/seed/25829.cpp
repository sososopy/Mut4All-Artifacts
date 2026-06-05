
template<int ... Dims>
class weeble {
    static constexpr int Ranks[sizeof...(Dims)] = {Dims...};
    const int rank = sizeof...(Dims);
};

weeble<3,5> x;

