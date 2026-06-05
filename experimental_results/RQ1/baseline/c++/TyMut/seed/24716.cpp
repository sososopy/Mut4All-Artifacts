

class Exc : public std::exception {
public:
    virtual const char* what() const throw()
    { return "test exception"; }
};

class Test {
public:
    Test()
    { } 

    ~Test()
    {   
        throw Exc();
    }   
};


int main()
{
    try {
        {
            Test();
        }
    }   
    catch (Exc& exc)
    {   
        std::cout << exc.what() << std::endl;
    }   
}

