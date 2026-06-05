
union Inc;
static_assert( !__has_virtual_destructor(Inc));
struct Inc2;
static_assert( !__has_virtual_destructor(Inc2[1]) );

