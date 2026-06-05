
template<int>
class TS {
    int x;
public:
    constexpr TS(int) {}
};
TS(int) -> TS<1>;

template<TS> void foo() {}  // #1
template<int> void foo() {}  // #2

void test() { foo<2>(); }
