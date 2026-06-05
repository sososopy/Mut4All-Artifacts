
struct NonDefault
{
    NonDefault(int) {}
};

struct Base
{   
    Base(int) {}
};

struct Derived : public Base
{
    NonDefault foo = 4;    
    
    using Base::Base;
};

auto test()
{
    auto d = Derived{ 5 };
}


