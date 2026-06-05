
class MyObject
    {
    public:
        MyObject() {}
    };

    class MyClassSlow
    {
        MyObject array[30000] {}; 
    };


    int main()
    {
        MyClassSlow slow;
    }

