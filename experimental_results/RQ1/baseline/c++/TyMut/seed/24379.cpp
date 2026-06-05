
template <typename T> struct type_is_unsigned_int { static const bool value = false; };
template <> struct type_is_unsigned_int<unsigned int> { static const bool value = true; };

enum unscoped_unfixed_size_enum {
        some_enum_name = 0,
};

static_assert(
        static_cast<unscoped_unfixed_size_enum>(-1) <
                static_cast<unscoped_unfixed_size_enum>(0),
        "This assert doesn't fire, indicating that the enum is signed.");
static_assert(
        static_cast<__underlying_type(unscoped_unfixed_size_enum)>(-1) >
                static_cast<__underlying_type(unscoped_unfixed_size_enum)>(0),
        "But neither does this one!  It's inconsistent.");
static_assert(
        type_is_unsigned_int<__underlying_type(unscoped_unfixed_size_enum)>::value,
        "The exact type in the implementation is, in fact, \"unsigned int\".");
