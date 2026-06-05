
namespace A
{
    extern "C" void foo () throw ();
}

namespace B
{
    extern "C" void foo ();
}

int
main ()
{
    foo ();
}


