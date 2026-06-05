
template <class ... Ts>
void foo()
{
    []
    {
        using T = Ts;
    }();
}
template void foo<>();


