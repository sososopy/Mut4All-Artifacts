
struct aggressive_aggregate
{
    int a;
    int b;
};

int main()
{
    auto x = aggressive_aggregate(1,2); // OK 
    auto y = new aggressive_aggregate(1,2); // rejected, should be OK?
    //auto z = std::make_unique<aggressive_aggregate>(1,2); // rejected, should be OK?
}
