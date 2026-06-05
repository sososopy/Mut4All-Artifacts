

#include<type_traits>

using Lambda=decltype(+[](){});
template<typename T>
concept IsLambda=std::is_same<decltype(+[](){}), T>::value;

template<IsLambda L>
struct A{
    A(L lam){
        lam();
    }
};
auto lam=+[](){};
auto a=A<Lambda>(lam);


