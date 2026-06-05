
    // time g++ -std=c++14 -O1 -Wall initialization.cc
    class MyObject
    {
    public:
        MyObject() {}
    };
    
    
    class MyClassFast
    {
        MyObject array[30000];     // ok: fast to build with -O1
    };
    
    
    class MyClassSlow
    {
        MyObject array[30000] {};  // problem: super slow to build with -O1 (x100 slower than the fast version)
    };
    
    
    int main()
    {
    //    MyClassFast fast;
        MyClassSlow slow;
    }

