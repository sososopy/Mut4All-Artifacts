
struct A {};
template <typename T>
struct B
{
    A a;
    friend decltype(a);
};

int main()
{
    A a;
    B<int> b; 
}
