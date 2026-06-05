
template<class F, class... Args>
constexpr bool valid = (0 < sizeof(F{}(Args{}...)));


template<class F, class... Args>
void ovr(F f, Args... args)
requires valid<F, Args...> // external checker - fail, wrong Args formed
//requires (0 < sizeof(F{}(Args{}...))) // inline checker - ok (are wrong Args compensated by SFINAE?)
{
    return ovr<F, Args...>(int{}, f, args...); // a second ovr is expected to be called, but the compiler tries to call this ovr again. Collision between 'int' and 'F' is ignored, extra Args appended from 'f'
}

template<class F, class... Args> void ovr(int, F, Args...){}


void use()
{
    ovr([]{return 'c';});
}
