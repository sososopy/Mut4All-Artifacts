# 1 "test.cpp" 1
namespace boost {
    template<typename > struct result_of;
    template<typename F > struct result_of< F () >;

    namespace fusion
    {
        template <class, bool = true>
        struct unfused
        {
        };
    }

    template<class F>
    struct result_of< boost::fusion::unfused<F> () >;
}

struct test_func { };
typedef boost::result_of< boost::fusion::unfused< test_func >() >::type t;
