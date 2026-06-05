
#include <stdio.h>

struct ParamOne {
    double val {0.0};
};

struct ParamTwo {
    int val {0};
};

template<typename P, typename Data, typename Other>
class Baseclass
{
public:
    using subclass_type = P;
    using data_type = Data;
    using other_type = Other;

    bool Method( const Data &data);
	
public:
	int m_BaseClassValue { 304 };
};

template<typename P, typename Data, typename Other> using pdata_type = typename P::data_type;
template<typename P, typename Data, typename Other> using pother_type = typename P::other_type;

template<typename P, typename Data, typename Other>
bool Baseclass<P, Data, Other>::Method( const Data &data )
{
    P& Subclass = static_cast<P&>( *this );
    pother_type<P, Data, Other> other;
    other.val = 11;

    return Subclass.SubclassMethod( data, other );
}

template<typename Data, typename Other>
class Subclass : public Baseclass<Subclass<Data, Other>, Data, Other>
{
public:
    using data_type = Data;
    using other_type = Other;

    bool SubclassMethod( const Data &data, Other &other );
	
public:
	int m_SubClassValue { 472 };
};

template<typename Data, typename Other>
bool Subclass<Data, Other>::SubclassMethod( const Data &data, Other &other )
{
    return true;
}

template<>
bool Subclass<ParamOne, ParamTwo>::SubclassMethod( const ParamOne &data, ParamTwo &other )
{
    printf( "The this pointer is %lx with a size of %ld and values of %d and %d\n", (long)this, sizeof(*this), m_BaseClassValue, m_SubClassValue );
    return true;
}

int main(int argc, char **argv)
{
    ParamOne one;
    one.val = 5.0;

    Baseclass<Subclass<ParamOne, ParamTwo>, ParamOne, ParamTwo> test;

    test.Method(one);
    return 0;
}
