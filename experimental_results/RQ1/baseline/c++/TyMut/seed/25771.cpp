
class ClassA
{
public:
    virtual void meth(int n);
    virtual void meth(const char* s, bool q);
};

class ClassB : public ClassA
{
public:
    virtual void meth(int n);
};

void ClassA::meth(int n)
{
}

void ClassA::meth(const char* s, bool q)
{
}

void ClassB::meth(int n)
{
}


int main(int argc, char** argv)
{
    const char* param = "foobar";
    ClassB inst;

    inst.meth(param, false);

    return 0;
}

