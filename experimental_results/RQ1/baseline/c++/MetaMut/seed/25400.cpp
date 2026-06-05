
template <class>
concept bool Dummy = true;

template <typename>
class example {
    template <Dummy<> U>
    friend auto func();
};

class test {
    test() = default;
};

int main()
{
    test t;
}



