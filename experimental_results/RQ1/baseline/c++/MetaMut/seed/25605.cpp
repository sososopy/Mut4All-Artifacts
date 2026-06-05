
template<const float* b>
struct h
{
    static constexpr float value()
    {
        return *b;
    }
};

constexpr float a = 3.0f;

int main()
{
    cout<<a<<endl;                //3
    cout<<h<&a>::value<<endl;     //1
    return 0;
}
