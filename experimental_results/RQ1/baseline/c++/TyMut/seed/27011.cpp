
constexpr auto fun(int n)
{
    switch(n) {
    case 0: return 0;
    default: return throw 42, 42;      
    }
}
    
int main()
{
    static_assert(fun(0) == 0, "");    
}
