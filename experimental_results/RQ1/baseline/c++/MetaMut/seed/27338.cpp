
enum struct enu : int {
    o = 0,
    p = 1,
};
constexpr enu& operator|=(enu& a, enu b) {
    return a = enu(static_cast<int>(a) + static_cast<int>(b));
}
struct s {
    enu e = enu::o;
};
template<typename T, int N> struct my_array {
    T data[N];
};
static constexpr auto make_ss = []{
    my_array<s, 1> ss = {};
    //ss.data[0] = {}; //work-around to avoid ICE on next line
    ss.data[0].e |= enu::p; //ICE
    return ss;
}();
