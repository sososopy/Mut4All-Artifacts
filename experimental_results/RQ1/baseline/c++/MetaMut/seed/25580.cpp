
struct A
{
    template <typename t>
        void f()
        {
            int t = 1; // Error. g++ does NOT complain
        }

    template <typename t>
        void g();
};

template <typename t>
void A::g()
{
    int t = 2; // OK. g++ DOES complain
}

// (Non-template) Members of template class
template <typename t>
struct B
{
    void f()
    {
        int t = 3; // OK. g++ DOES complain
    }

    void g();
};

template <typename t>
void B<t>::g()
{
    int t = 4; // OK. g++ DOES complain
}


// Template members of template class
template <typename t>
struct C
{
    template <typename s>
    void f()
    {
        int t = 5; // Error. g++ does NOT complain
        int s = 6; // Error. g++ does NOT complain
    }

    template <typename s>
    void g();
};

template <typename t>
template <typename s>
void C<t>::g()
{
    int t = 7; // OK. g++ DOES complain
    int s = 8; // OK. g++ DOES complain
}
