
struct S;
using F1 = int (S::*)();
using F2 = int (S::*)() noexcept;

auto v = true ? F1() : F2();
auto w = true ? F2() : F1();
