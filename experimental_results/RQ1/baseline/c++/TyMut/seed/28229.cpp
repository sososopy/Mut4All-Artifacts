
template <typename DT>
struct AT
{
    static void cn() noexcept(noexcept(DT::CN()));
    void SNFP( void *n ) noexcept(noexcept(cn()));
};
