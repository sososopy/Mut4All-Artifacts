
template<class T,class First=char>
void foo(First,T){}
template<>
void foo<double>(int,double){}

