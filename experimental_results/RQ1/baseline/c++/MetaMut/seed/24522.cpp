
namespace N
{
template <typename T1>
void
f (const T1) {}
}

template <>
void
N::f (const int) {}

