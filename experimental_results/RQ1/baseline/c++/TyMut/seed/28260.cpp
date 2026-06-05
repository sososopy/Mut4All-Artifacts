
struct f{f();};
consteval auto MakeHash ()
{
    if (1) ;
    return [] (f str) { };
}

void bar(f arg)
{
    auto hash = MakeHash ();
    hash(arg);
}
