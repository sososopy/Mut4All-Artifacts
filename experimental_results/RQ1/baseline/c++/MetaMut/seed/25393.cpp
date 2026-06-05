

namespace std {class type_info { };}
[[noreturn]] void f1(const std::type_info& type);
struct S1{~S1();};
struct S2
{
    virtual S1 f2() const { f1(typeid(*this)); } 
    S1 f3() const { return f2(); } 
};
void f4() { S2 a; a.f3(); } 


