template < class T, class V > struct A
{
    A ();
    A (A &);
    A (A < V,T >);
};

void f () 
{
    A <int, int> (A < int, int >());
}