
struct p {};
struct b { b( p && ) {} };
struct d : b { using b::b; };

d o( p{} );

