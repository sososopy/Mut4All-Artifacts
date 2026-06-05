

#include <cstddef>

struct ErrorConversionNullBug
{
    ErrorConversionNullBug()
        : n(NULL)
        , c('c')
    {}
    unsigned int n;
    const char c;
};

int main()
{
    ErrorConversionNullBug bug;
}

