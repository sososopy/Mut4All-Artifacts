
template <class T>
class C {
private:
    template <T a, T b>
    struct Implementation;
    template <T b>
    struct Implementation<0, b> { typedef void Typedef; };
public:
    typedef typename Implementation<0, 0>::Typedef Type;
};

template class C<unsigned>;
