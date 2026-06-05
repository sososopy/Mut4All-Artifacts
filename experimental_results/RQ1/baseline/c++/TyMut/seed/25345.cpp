
    struct X
    {
        explicit operator int() { return 0; }
    };
    
    int main()
    {
    	auto y = int{X{}};
    }



