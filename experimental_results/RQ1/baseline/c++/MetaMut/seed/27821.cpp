
struct A { int a[3]; };

template <A> struct B { };

void f (B<A{ }>) { }
void f (B<A{ 0 }>) { }
void f (B<A{ 0, 0 }>) { }
void f (B<A{ 0, 0, 0 }>) { }
