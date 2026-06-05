
template <typename Tp>
class int_v4
{
    typedef Tp Tpv __attribute__((vector_size(4 * sizeof(Tp))));
    Tpv v;

    static Tpv shuffle(const int_v4& u1, const int_v4& u2, const int_v4& s) {
        return __builtin_shuffle(u1.v, u2.v, s.v); //segfault
    }

    Tpv shuffle2(const int_v4& u1, const int_v4& u2, const int_v4& s) {
        return __builtin_shuffle(v, v, v); //error if shuffle() is removed
    }
};

int main()
{
    int_v4<int> iv4;
    int_v4<unsigned int> uv4;
    return 0;
}
