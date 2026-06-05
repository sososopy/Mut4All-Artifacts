
#include <boost/signals2.hpp>

class A
{
public:
    A()
    {
        // these two work
        sig2.connect( [this]() {f();} );
        sig.connect( [this](auto x) {this->g(x);} );
       
        // this does not work
        sig.connect( [this](auto x) {g(x);} );
    }


private:
    void f();
    void g(int);

    boost::signals2::signal<void (int)> sig;
    boost::signals2::signal<void ()> sig2;
};

