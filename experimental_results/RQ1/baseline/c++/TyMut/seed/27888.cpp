
struct Q { };
struct S { };
struct T { };
struct U: S, T { };   // not a standard-layout class

static_assert (!__is_standard_layout (U));
