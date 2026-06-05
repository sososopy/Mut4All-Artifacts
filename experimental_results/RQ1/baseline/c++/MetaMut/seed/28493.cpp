
enum Cat { prvalue, lvalue, xvalue };

template<typename T>
struct value_category {
    // Or can be an integral or enum value
    static constexpr auto value = prvalue;
};

template<typename T>
struct value_category<T&> {
    static constexpr auto value = lvalue;
};

template<typename T>
struct value_category<T&&> {
    static constexpr auto value = xvalue;
};

// Double parens for ensuring we inspect an expression,
// not an entity
#define VALUE_CATEGORY(expr) value_category<decltype((expr))>::value

constexpr bool global = true;
static_assert( VALUE_CATEGORY(!(!global)) == prvalue, "GCC gets this right" );

int main()
{
  bool b = true;
  if ( VALUE_CATEGORY(!(!b)) != prvalue )
    throw 1;
}
