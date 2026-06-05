
int get_int();

#define EXPR get_int()

template<int>
static void check();

template<class T>
static auto test(decltype(check<(EXPR, T())>(), T())) -> char(&)[1];

template<class>
static auto test(...) -> char(&)[2];

static_assert(sizeof(test<int>(0)) != 1, "Ouch");
