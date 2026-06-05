
class A
{
    public: 
    A& operator++(){return *this;}
};

class B
{
    public:
    B operator++(int){return *this;}        
};

class C:public A, public B
{
};

int main()
{
    C c;
    c++;
    ++c;
}

