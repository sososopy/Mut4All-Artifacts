
inline bool assert_static_check( bool ev )
{
    return __builtin_constant_p( ev ) && !ev;
}

struct X1
{
    int v1_;
};

inline bool operator==( X1 const& x1, X1 const& x2 )
{
    return x1.v1_ == x2.v1_;
}

int compare( X1 s1, X1 s2 )
{
    return assert_static_check( s1 == s2 );
}

struct X2
{
    int v1_;
    int v2_;
};

inline bool operator==( X2 const& x1, X2 const& x2 )
{
    return x1.v1_ == x2.v1_ && x1.v2_ == x2.v2_;
}

int compare( X2 s1, X2 s2 )
{
    return assert_static_check( s1 == s2 );
}
