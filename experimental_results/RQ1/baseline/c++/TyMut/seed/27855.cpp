
template <class ... Ts>
void foo()
{
    (
        []
        {
            static_cast<Ts*>(nullptr);
        }()
        , ...
    );
}
template void foo<>();

