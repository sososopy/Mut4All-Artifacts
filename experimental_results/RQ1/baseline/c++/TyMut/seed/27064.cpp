
#include<type_traits>

template<int...> struct VsA{};

template<class ValueType>
struct ValueTemplate
{
    template<template<ValueType...TypesA> class TemplateA, class T           > struct IsInstanceOf                                  : std::false_type{};
    template<template<ValueType...TypesA> class TemplateA, ValueType...TypesA> struct IsInstanceOf<TemplateA, TemplateA<TypesA...>> : std:: true_type{};
};

static const bool foo = ValueTemplate<int>::IsInstanceOf<VsA,VsA<0>>::value;

int main(){return 0;};
