
struct A 
{
  A(...);
};

template<typename... T> struct B : T...
{
  B() : T([]{})... {}
};

B<A> b;
