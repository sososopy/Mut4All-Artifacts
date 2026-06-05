
int counter = 0;

struct canary {
    canary() {}

    bool active = false;

    canary(canary const&) = delete;
    canary(canary&& other): active(other.active) { other.active = false; }

    ~canary()
    { if(active) ++counter; }
};

struct optional_base {
    union {
        unsigned char empty {};
        canary val;
    };
    bool active = false;

    optional_base(canary val)
        : val(static_cast<canary&&>(val))
        , active(true)
    {}

    ~optional_base()
    {
        if(active) {
            val.~canary();
        }
    }
};

struct optional: optional_base {
    using optional_base::optional_base;
};

int main()
{
    {
        // N.B. inactive from the start
        canary c;
        //assert( !c.active );

        optional o(static_cast<canary&&>(c));

        //assert( !c.active );
        // may fire if -O level above 0
        //assert( !o.val.active );
    }

    return counter;
}
