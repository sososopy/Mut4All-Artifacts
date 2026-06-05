
template < typename = void > 
class A 
{
public:
 A () : f0 ( 1 ) { }
 A (int);

private:
 typedef A<> f0;
};

A<> a;

