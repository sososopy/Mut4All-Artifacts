
struct S
{
//  S () = default;

    S (const S&) = delete;
    S& operator= (const S&) = delete;
};

S x;


