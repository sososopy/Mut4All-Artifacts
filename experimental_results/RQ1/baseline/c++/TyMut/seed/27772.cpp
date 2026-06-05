
template<typename T>
struct Y {
    ~Y() requires(true) = default;
    ~Y() requires(false) {}
};

