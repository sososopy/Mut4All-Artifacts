
template <typename>
class X {
   
    void f(){}
};
class Y
{
    friend void X<int>::f();  
};

int main()
{
    X<int> t;
    t.f();
    Y b;
}
