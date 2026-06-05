
template<decltype(sizeof(int)) N>
struct A{
        template<class TB>
        struct B{
                using TB_Alias=TB;
                template<class TC=TB_Alias>
                struct C{
                        typedef TC Arr[N];
                        Arr arr;
                };
        };
};
template<decltype(sizeof(int)) N, class TB>
void f(const decltype(A<N>::template B<TB>::template C<>::arr)){}

template<>void f<3, char>(const char [3]){}


