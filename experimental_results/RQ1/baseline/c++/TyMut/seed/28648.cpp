
struct A 
{
    unsigned int a : 8; /* no crash for sizes from 1 to 7 and 32 */

    constexpr bool operator==(const A&) const = default;
};


int main()
{
    A a{}, b{};
    
    return a == b;
}


