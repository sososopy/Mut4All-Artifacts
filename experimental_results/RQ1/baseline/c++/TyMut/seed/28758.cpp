

template< class T
        , bool = requires { typename T::pointer; }
        >
struct p { using type = void; };

template< class T >
struct p<T, true> { using type = T::pointer; };

template< class T > using P = typename p<T>::type;


