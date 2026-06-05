
int i;
static_assert( i || true, "" );
static_assert( ! ( i && false ), "" );
