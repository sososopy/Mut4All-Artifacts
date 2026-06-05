

#include <vector>

namespace ossia
{
    struct value;

    struct value_variant_type {
        union Impl {
            std::vector<ossia::value> m_value8;
            Impl() { }
            ~Impl() { }
        } m_impl;

        value_variant_type() { }
        value_variant_type(const value_variant_type&) { }

        value_variant_type(const std::vector<ossia::value>& v); 
        value_variant_type(std::vector<ossia::value>&& v);

        value_variant_type& operator=(const value_variant_type&)
        { return *this; }

        value_variant_type& operator=(value_variant_type&& other);
    };

    struct value {
        value_variant_type v;

        value() { }

        value(const std::vector<ossia::value>& val) noexcept : v{val}
        {
        }

        explicit value(std::vector<ossia::value>&& val) noexcept : v{std::move(val)}
        {
        }
    };


    inline value_variant_type::value_variant_type(const std::vector<ossia::value>& v)
    {
        new (&m_impl.m_value8) std::vector<ossia::value>{v};
    }

    inline value_variant_type::value_variant_type(std::vector<ossia::value>&& v)
    {
        new (&m_impl.m_value8) std::vector<ossia::value>{std::move(v)};
    }


    value_variant_type& value_variant_type::operator=(value_variant_type&& other)
    {
        new (&m_impl.m_value8)
            std::vector<ossia::value>{std::move(other.m_impl.m_value8)};
        return *this;
    }

    inline ossia::value init_value()
    {
        return std::vector<ossia::value>{};
    }

    void create_value_inline(ossia::value& v) {
        v = ossia::init_value();
    }
}

int main()
{
    ossia::value v;
    create_value_inline(v);
}


