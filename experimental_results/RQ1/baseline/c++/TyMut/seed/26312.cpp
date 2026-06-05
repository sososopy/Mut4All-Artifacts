
struct A { int &&temporary; int x, y; };
int f();
const A &c = { 10, ++c.temporary };

