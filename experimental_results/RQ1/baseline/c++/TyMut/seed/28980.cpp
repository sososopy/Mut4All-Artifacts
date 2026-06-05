
template<int v> using __bool_constant=int;

template<class R>
struct duration { 

    static constexpr int fun()
    { 
        return 0;
    } 

    template<typename _Period2>
        using __is_harmonic
        = __bool_constant<(&duration::fun)()>;
    template <class _P2>
        static constexpr __is_harmonic<_P2> a=55;
};
int main(){
    duration<int> g;
    return g.a<int>;

}
