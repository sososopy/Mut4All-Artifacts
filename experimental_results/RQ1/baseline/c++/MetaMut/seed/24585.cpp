
template<typename T> struct X { };

template<typename Ftype, int x, int y>
void fCall(Ftype & f)
{
    f.template X<x, y>(); // compiler stops here, confuses f's 
                          // member template X with the template
                          // class X defined above  
}

// To be used like this:

struct F
{
    template<int x, int y> void X() { }
};

int main()
{
    F f;
    fCall<F, 1, 2>(f);
}
