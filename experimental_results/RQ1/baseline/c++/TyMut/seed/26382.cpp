
template< typename T >
T declval() noexcept;

struct base
{
    virtual ~base() {}
};

class outer :
    public base
{
protected:
    class nested
    {
    private:
        outer& m_outer;

    public:
        explicit nested(outer& o) noexcept(noexcept(o.on_nested_ctor())) :
            m_outer(o)
        {
            o.on_nested_ctor();
        }

        nested(nested const&) = delete;
        nested& operator= (nested const&) = delete;

        ~nested() noexcept(noexcept(declval<outer&>().on_nested_dtor()))
        {
            m_outer.on_nested_dtor();
        }
    };

public:
    ~outer() {}

protected:
    void on_nested_ctor();
    void on_nested_dtor();
};

