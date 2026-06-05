
template<typename TDERIVED>
struct __a_t
{
       unsigned char A = 0;
       constexpr TDERIVED & SetA(const unsigned char & value) { A = value;   return *static_cast<TDERIVED *>(this); }

       constexpr __a_t() {}
};

template<typename TDERIVED>
struct __b_t
{
       unsigned char B = 0;
       constexpr TDERIVED & SetB(const unsigned char & value) { B = value; return *static_cast<TDERIVED *>(this); }

       constexpr __b_t() {}
};

class __ab_t: public __a_t<__ab_t>, public __b_t<__ab_t>
{
    public:
        constexpr __ab_t(){}

};

static constexpr const auto AB = __ab_t().SetA(100).SetB(10);

int A = 0;

int main()
{
    A = AB.A;
}

