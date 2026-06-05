
struct IA
{
    virtual ~IA() {}
};

struct IB
{
    virtual ~IB() {}
};

struct IC: virtual IA, virtual IB {};

struct CA : virtual IA {};

struct CB: virtual IB {};

struct CC: virtual IC, CA, CB {};

int main()
{
    CC c;
    return 0;
}