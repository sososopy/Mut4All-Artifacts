
template<typename T>
constexpr T const& f(T const& x) { return x; }

struct X {};
struct Y : X {};
struct Z : Y { constexpr Z() {} };　/*user-declared constructor*/

static constexpr auto z = f(Z());
