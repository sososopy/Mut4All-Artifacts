
#include <utility>

class WrappedValue
{
    public:
        constexpr explicit WrappedValue(double val): m_value{val}{}

        constexpr bool operator<=>(WrappedValue const&) const = default

        constexpr double value() const
        {return m_value; }

    private:
        double m_value;
};

