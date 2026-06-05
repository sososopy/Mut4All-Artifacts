
    template <class ValueType>
    class base_t
    {
    public:
            int a;
    };
     
    template <class ValueType, class DerivedType = ValueType>
    class derived_t :
            public base_t<ValueType>
    {
    public:
            int foo()
            {
                    a = 1;          // this does not work. why?
                    this->a = 1;    // this works
                    return 0;
            }
    };
     
    int main(int argc, char *argv[])
    {
            derived_t<int> a;
            a.foo();
            return 0;
    }
