
template<int i> class TC { };
constexpr int foo[] = { 42, };
TC<foo[0 > 1]> bar;

