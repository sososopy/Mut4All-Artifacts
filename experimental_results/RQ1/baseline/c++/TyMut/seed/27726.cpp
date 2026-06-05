

class Test // non-copyable and non-movable class with virtual functions
{
public:
    Test() = delete;

    Test(const Test&) = delete;
    Test(Test&&) = delete;
    Test& operator=(const Test&) = delete;
    Test& operator=(Test&&) = delete;

    Test(int a, int b) : a_(a), b_(b) {}
    virtual ~Test() {}

    int a_;
    int b_;
};

//----------------

class B
{
public:
/*(1)*/ B() : test_{{1, 2}, {3, 4}} {} // Does not compile on GCC

private:
        Test test_[2];
};

//---------------- 

int main()
{
    B b;
    Test test[2] = {{1, 2}, {3, 4}}; // Successfully compiles
}

