struct A
{
    unsigned int a:5;    
};

constexpr std::strong_ordering operator<=>(const A & left, const A & right ) 
{
    return left.a <=> right.a;
}

