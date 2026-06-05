
struct i { static constexpr int m = 5; };

template< int ... p >
struct g {
    static constexpr int m = i::m - ( sizeof ... ( p ) );
    int n = ( sizeof ... ( p ) );
};

template< int ... p >
struct d {
    static constexpr int m = ( i::m - sizeof ... ( p ) );
    int n = sizeof ... ( p );
};
