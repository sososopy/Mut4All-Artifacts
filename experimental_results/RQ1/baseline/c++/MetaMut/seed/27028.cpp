
template<typename TF>
void call(TF&& f)
{
    f(1);   
}

struct Example
{        
    void foo(int){ }

    void bar()
    {
        call([this](auto x){ foo(x); });
    }
};

int main()
{
    Example{}.bar();
    return 0;
}

