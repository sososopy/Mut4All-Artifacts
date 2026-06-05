
template <typename R>
class du {
    R r;
public:
    constexpr du() = default;
    du(const du&)  = default;

    template<typename R2>
    constexpr explicit du(const R2& rep)
    : r(static_cast<R>(rep)) { }
};

typedef du<int> secs;

class DWP {
    secs to;
public:
    DWP(secs x);
};

DWP::DWP(secs x)
: to(x)
{}

class FHT {
public:
    FHT()
    : dwp{secs(READ_TIMEOUT)}
    {}
protected:
    static auto constexpr READ_TIMEOUT = 1;
    DWP dwp;
};

int main()
{
    FHT fht;
}

