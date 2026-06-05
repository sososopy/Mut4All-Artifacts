
#include <thread>
#include <iostream>

struct Thread                                                                           : std::thread
{
    Thread() = default;
    template <class F, class... P> Thread(F&& task, P&&... p)
    : std::thread(
         [&task] (P&&... p) { task(std::forward<P> (p)...); }, 
         std::forward<P> (p)...
      )
    {}
    Thread& operator = (Thread&& t)
    {
        (std::thread&)*this = std::move((std::thread&)t);
        return *this;
    }
};

struct A
{
    int a;
    Thread t;

    A()
    {
        t = [this] { std::cerr << &a << " \n"; };
    }
    ~A()
    {
        t.join();
    }
};

int main()
{
    A a;
    std::cerr << &a.a << " \n";
    return 0;
}
