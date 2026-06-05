
    int main()
    {
        auto f = [](auto&&...)
        {
        };

        non_copyable nc;
        
        f(nc); // ok on both compilers
        f(nc, int{}); // ok on both compilers
        f(int{}, nc); // error only on gcc 
    } 

