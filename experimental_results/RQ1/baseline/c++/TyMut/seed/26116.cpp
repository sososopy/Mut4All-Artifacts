

template<class T, class U>
struct Foo { };

template<class... Args>
void Bar(Foo<Args...> f)
{
}

template<class... Args>
void Baz()
{
    Foo<Args...> f;
}

int main()
{
    Foo<int, float> f;
    Bar(f);                    // #1
    Bar<int, float>(f);        // #2     
    Baz<int, float>();         // #3
}

