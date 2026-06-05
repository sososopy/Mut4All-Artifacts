struct X 
{
    int x; 
    constexpr int get() const {return x;} 
    constexpr void set(int foo) {x = foo;}
}; 

constexpr int bar() 
{
    X x{42}; 
    x.set(666); 
    return x.get();
} 

int main() 
{
    constexpr int foo = bar();
}

