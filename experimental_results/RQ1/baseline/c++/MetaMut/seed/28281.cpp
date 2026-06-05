
class Outer
{
private: //  !!!!!
    struct Inner
    {};
};

template<typename T>
struct Meow
{
    void purr() {
        Outer::Inner oi;
    }
};

int main()
{
    Meow<double> kitty;
    //kitty.purr();
}

