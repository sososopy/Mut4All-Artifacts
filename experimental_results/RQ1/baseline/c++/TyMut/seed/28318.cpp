
struct Context
{
    void
    action() const
    {
        struct
        {
            int wrapped;
            decltype( & wrapped ) get() { return &wrapped; }
        } t;

        *t.get()= 42;
    }
};

