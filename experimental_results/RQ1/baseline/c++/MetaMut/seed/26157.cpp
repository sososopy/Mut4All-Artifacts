

#include <iostream>
#include <string>
#include <exception>
#include <sstream>

#define DUMP(x) std::cout << #x << " is " << x << std::endl

class exception : public std::exception
{
public:
    exception()
        : std::exception()
    {}

    exception(const exception &rhs)
        : std::exception(rhs)
    {
        errmsg.str(rhs.errmsg.str());
    }


    virtual ~exception() throw()
    {}

    virtual const char* what() const throw()
    {
        return errmsg.str().c_str();
    }


    template<typename T>
    exception& operator<<(T const& v)
    {
        this->errmsg << v;
        return *this;
    }

private:
    std::ostringstream errmsg;
};

struct thing_t
{
    int a;
};

template<typename OS>
OS& operator<<(OS& oss, const thing_t& t)
{
    oss << "thing is: " << t.a;
    return oss;
}


void do_thing(thing_t& th)
{
    // throw exception() << th; //ok all clang++-3.3 clag-3.4 g++4.7
     throw exception() << "some text " << th; // ok g++ 4.7, ok clang++-3.3 fails to compile on clang++-3.4
}

int main(int argc, char **argv)
{
    thing_t the_thing;
    the_thing.a = 8;

    try
    {
        do_thing(the_thing);
    }
    catch(exception& e)
    {
        std::cout << "e.what()" << " is " << e.what() << std::endl;
    }

}
