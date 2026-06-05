
struct Dummy
{};

template <typename T>
class Base
{
    public: template< typename S>
            void do_sth(S param, struct Dummy dummy);
};

template< typename T>
template< typename S>
//void Base<T>::do_sth(S param, Dummy)        // working
void Base<T>::do_sth(S param, struct Dummy) // not working
{
}
