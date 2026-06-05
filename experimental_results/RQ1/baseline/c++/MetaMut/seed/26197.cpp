
template<class T>
class Foo
{
    void process(Foo m);    
    void bar()
    {
        [this] { process(Foo()); }();
    }
};

template class Foo<int>;
