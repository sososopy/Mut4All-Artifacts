
#include <cassert>

struct holder { int& value; };

constexpr holder from_value(int& value)
{ return { value }; }

struct aggr { int i; };

constexpr holder from_aggr(aggr& a)
{ return from_value(a.i); }

int main()
{
    aggr a { 42 };

    // these don't fire
    assert( &from_value(a.i).value != nullptr );
    assert( &a.i == &from_value(a.i).value );

    // those do
    assert( &from_aggr(a).value != nullptr );
    assert( &a.i == &from_aggr(a).value );
}
