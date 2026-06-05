
template < typename > struct A
{
    A (A const &) = delete;
    A (int);
};

template < typename X > struct B
{
    A < X > a { 0 };
};

B < int >b;
