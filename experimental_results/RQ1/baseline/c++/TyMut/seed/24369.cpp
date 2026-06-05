
typedef int & T;

T f (float const & arg) 
{
    return T (arg); // XXX initializes int & from float const
}

int main ()
{
    float const a (1);
    T t (f (a));
}

