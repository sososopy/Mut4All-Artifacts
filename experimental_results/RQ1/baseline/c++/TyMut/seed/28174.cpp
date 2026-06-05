
struct incomplete;

// fails on clang, OK on GCC
const bool res = __is_constructible(incomplete);

// GCC: invalid use of incomplete type 'struct incomplete'
const bool res0 = __is_trivial(incomplete);

// GCC: invalid use of incomplete type 'struct incomplete'
const bool res1 = __is_final(incomplete);

