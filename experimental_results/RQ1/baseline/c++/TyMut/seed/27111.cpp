
struct empty {};

union no_attribute_t
{
  empty _0;
  empty _1;
};

union with_attribute_t
{
  [[no_unique_address]] empty _0;
  [[no_unique_address]] empty _1;
};

constexpr no_attribute_t no_attribute{};
constexpr with_attribute_t with_attribute{};

// This succeeds
static_assert( &no_attribute._0 == &no_attribute._1 );

// This fails
static_assert( &with_attribute._0 == &with_attribute._1 );
