
class  A
{
    protected:
        int s;
    public:
        A();
};

class  B : public A
{
    protected:
    public:
        B();

    // By reversing the order of these two declarations
    // you can change the location of the error
    friend void foo(B &, int);
    friend void foo(B &, B& b);
};

void foo(B& a, int)
{
    // This is only fine if this function was the first "friend" declared
    a.A::s = 3;
    // Always fine
    a.s = 3;
}

void foo(B& a, B& b)
{
    // This is only fine if this function was the first "friend" declared
    a.A::s = 3;
    // Always fine
    a.s = 3;
}
