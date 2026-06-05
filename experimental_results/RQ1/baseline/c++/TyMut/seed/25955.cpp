
class A
{   
    private:
        int a;
        const int* const aptr;

    public:
        constexpr A( int _a):
            a(_a)
           , aptr( &a)           // why aptr could not be initialized? 
    {}  
};  

class Data { } d1; 

class B
{   
    private:
        Data* dptr1;

    public:
        constexpr B(Data* _p): dptr1( _p) {}

};  

class Use 
{   
    static constexpr A a{2};   // fail! error: field initializer is not constant
    static constexpr B b{&d1}; // works
};
