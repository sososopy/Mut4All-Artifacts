
template< typename T >
struct plus {};

template< typename Q >
struct test
{
    template< template< typename > class BinaryFunT, typename NumberT >
    static void test_impl(NumberT n)
    {
    }

    static void test_arith()
    {
        test_impl< plus >(10);
    }
};

