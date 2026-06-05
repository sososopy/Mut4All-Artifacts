
struct X
{
    int i_;
    bool b_;

    int& test() {return b_ ? i_ : throw 1;}
};

