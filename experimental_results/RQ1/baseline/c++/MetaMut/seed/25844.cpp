
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class S
{
public:
    S():
    _str("")
    {
        cout << "New S with address " << this << endl;
    }

    S(const std::string str):
    _str(str)
    {   
        cout << "New S with address " << this << endl;
    }
    
    S(const S& val)
    {
        cout << "New copy of S with address " << this << endl;
        _str = val._str;
    }

    S& operator=(const S& val)
    {
        cout << "Assign to S with address " << this << endl;
        _str = val._str;
        return *this;
    }

    ~S()
    {   
        cout << "Delete S with address " << this << endl;
    }

private:
    std::string _str;
};

vector<S> vs;

S someVectorChanges()
{
    vs.resize(5);
    vs[0] = S("foo");
    return vs[0];
}

int main()
{
    vs.resize(4);
    vs[1] = someVectorChanges();
    return 0;
}

