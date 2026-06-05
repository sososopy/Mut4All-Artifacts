
struct x0
{
    x0 () = default;
};
struct x1
{
    x0 x2[];
    void x3 ()
    {
        x1 ();
    }
};

