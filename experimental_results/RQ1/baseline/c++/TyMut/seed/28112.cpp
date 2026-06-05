
class Test
{
public:
    virtual void foo(){}
    void operator delete(void*)=delete;
};
int main()
{
    return 0;
}

