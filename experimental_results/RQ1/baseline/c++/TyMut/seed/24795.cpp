
#include<iostream>
#include<string>

template<typename T>
class Holder{
    typedef T value_type;
    value_type m_v;
public:
    Holder(const value_type& v):m_v(v){}
    operator const value_type() const{return m_v;}
    operator value_type(){return m_v;}

//    operator const value_type&() const{return m_v;}
//    operator value_type&(){return m_v;}
};

struct A {
    Holder<std::string> m_i;
    A(const std::string& i):m_i(i){}
};


void bad_const_access(const A* a){
    std::cout << (std::string&)a->m_i << std::endl; //crash in gcc-13 but OK in gcc-12
}
void correct_const_access(const A* a){
    std::cout << (const std::string&)a->m_i << std::endl;
}
int main(){
    const A a("test");
    bad_const_access(&a);
    correct_const_access(&a);
    return 0;
}
