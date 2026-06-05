template<class T>
class confused{};

template<class T>
class test
{
    void foo() {
        if(this->b.confused < 1);
    };

    struct bar
    {
        int confused;
    } b;
};