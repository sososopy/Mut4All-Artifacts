
template<int z> class A {};

template<int x>
class B {
public:
    static const int y = (x != -1 ? 0 : 0);

    template<int z> void g(const A<y> &a) {
        const char *x = __func__;
    }
};
template void B<0>::g<0>(const A<0> &);

