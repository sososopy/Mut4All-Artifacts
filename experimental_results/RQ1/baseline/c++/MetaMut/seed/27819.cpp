
struct A;
namespace N { extern A a; }
struct A {} ::N::a;

struct A;
struct B { static A a; };
struct A {} ::B::a;
