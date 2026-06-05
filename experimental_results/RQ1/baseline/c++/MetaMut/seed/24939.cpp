
#include <stdexcept>
#include <iostream>

struct Interface_1
{
    virtual void setA() = 0;
};

struct Interface_2
{
    virtual void setB() = 0;
};

struct Master: Interface_1, Interface_2
{
    void setA()
    {
        throw std::runtime_error("I was thrown");
    }
    void setB()
    {
        throw std::runtime_error("I was thrown");
    }
};

int main()
{
    Master m;
    Interface_2& i2 = m;
    try
    {
        i2.setB();
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Caught exception e=" << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Did not catch exception" << std::endl;
    }

    std::cout << "Done running" << std::endl;
}


