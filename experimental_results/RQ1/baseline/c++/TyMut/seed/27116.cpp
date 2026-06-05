
struct expr
{
    int a, b;
    expr& operator=(const expr&) { return *this; }
};
#pragma GCC diagnostic pop

#include <type_traits>
static_assert(std::is_aggregate_v<expr>);

expr foo(expr e)
{
    return e;
}

