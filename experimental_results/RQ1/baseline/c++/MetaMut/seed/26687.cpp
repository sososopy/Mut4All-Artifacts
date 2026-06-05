

#include <array>

const size_t SIZE_3 = 3;

template <class T1, class T2> struct TemplateTest
{};

struct a_type;
struct another_type;
typedef int int_type;

struct Test
{
    enum { SIZE_1 = 1 };
    static const size_t SIZE_2 = 2;

#if 1
    // broken :
    std::array<void*, SIZE_1> array1 = std::array<void*, SIZE_1>({nullptr});
    std::array<void*, SIZE_2> array2 = std::array<void*, SIZE_2>({nullptr});
    std::array<void*, SIZE_3> array3 = std::array<void*, SIZE_3>({nullptr});

    TemplateTest<a_type, another_type> struct1 = TemplateTest<a_type, another_type> ();
    TemplateTest<a_type, int_type> struct2 = TemplateTest<a_type, int_type> ();
#else
    // workarounds :
    // use parentheses
    std::array<void*, SIZE_1> array1 = (std::array<void*, SIZE_1>({nullptr}));
    // explicitly write out the value
    std::array<void*, SIZE_2> array2 = std::array<void*, 2>({nullptr});
    // use a cast
    std::array<void*, SIZE_3> array3 = std::array<void*, static_cast<size_t>(SIZE_3)>({nullptr});

    // use a typedef
    typedef TemplateTest<a_type, another_type> fix_1;
    TemplateTest<a_type, another_type> struct1 = fix_1();
    // use decltype
    TemplateTest<a_type, int_type> struct2 = decltype(struct2)();
    // use int explicitly
    TemplateTest<a_type, int_type> struct2_alt = TemplateTest<a_type, int>();
#endif
};

