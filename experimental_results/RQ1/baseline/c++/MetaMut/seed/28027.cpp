
class A
{
    public:
    
    A(){};    
    A(const A &)=delete;
    ~A(){}
};

class B
{
    public:
    
    B() : a{}
    {}

    A a[1];
};

int main() 
{
    B b;
}

