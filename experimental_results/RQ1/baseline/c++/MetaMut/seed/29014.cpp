
auto a = []{};
auto b = []{};
static_assert(a != b); // error: '(<lambda()>::_FUN != <lambda()>::_FUN)' is not a constant expression
