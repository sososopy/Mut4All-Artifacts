
template <class T> concept Int = requires { T{0}; };
template <template <Int> class T> struct X        { };
template <Int>   /* ^~~ */        struct Y : X<Y> { };
                                  struct Z        { };
void f() { Y<Z> x; }
