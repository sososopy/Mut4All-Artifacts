template < typename >
struct Type
{ };

template < typename... Ts >
struct ImplT: Ts...
{ using Ts::operator()...; };

template < typename... Ts >
using ImplA = ImplT<decltype([](Type<Ts>){})...>;

template < typename... Ts >
constexpr
bool allUnique{([]( ImplA<Ts...> x, Type<Ts> t ) { x.operator()(t); return true;} && ...)};

static_assert(allUnique<int>);
//static_assert(not allUnique<int, int>);