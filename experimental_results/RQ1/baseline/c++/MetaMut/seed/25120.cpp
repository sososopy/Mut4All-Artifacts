
struct S1{};

template<typename,typename>
template<typename...>
using U=S1;

template<typename>
struct S2{
        template<typename,typename=U<>>
        struct S3;
        S3<S1>
};

