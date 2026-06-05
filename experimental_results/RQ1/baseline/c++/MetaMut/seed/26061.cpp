struct bad_date {};

// invariants:
//    1 <= d_
class day
{
    int d_;

    static
    constexpr
    int
    check_invariants(int d)
    {
        return 1 <= d ? d : throw bad_date{};
    }
public:
    constexpr
    explicit
    day(int d)
        : d_(check_invariants(d))
        {}

    constexpr
    operator int() const
        {return d_;}
};

// invariants:
//    1 <= m_ && m_ <= 12
class month
{
    int m_;

    static
    constexpr
    int
    check_invariants(int m)
    {
        return !(1 <= m && m <= 12) ? throw bad_date{} : m;
    }
public:
    constexpr
    explicit
    month(int m)
        : m_(check_invariants(m))
        {}

    constexpr
    operator int() const
        {return m_;}
};

// invariants:
//    none
class year
{
    int y_;
public:
    constexpr
    explicit
    year(int y)
        : y_(y)
        {}

    constexpr
    operator int() const
        {return y_;}

    constexpr
    bool
    is_leap() const
        {return y_ % 4 == 0 && (y_ % 100 != 0 || y_ % 400 == 0);}
};

static
constexpr
int limit[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// invariants:
//    1 <= m_ && m_ <= 12
class year_month
{
    year y_;
    month m_;

public:
    constexpr
    year_month(year y, month m)
        : y_(y),
          m_(m)
        {}

    constexpr
    year_month(year y, int m)
        : y_(y),
          m_(m)
        {}

    constexpr
    year_month(int y, month m)
        : y_(y),
          m_(m)
        {}

    constexpr
    ::year
    year() const
        {return y_;}

    constexpr
    ::month
    month() const
        {return m_;}

    constexpr
    int
    days_in_month() const
        {return m_ != 2 ? limit[m_-1] : y_.is_leap() + 28;}
};

// invariants:
//    1 <= m_ && m_ <= 12
//    if m_ in {1, 3, 5, 7, 8, 10, 12}
//        1 <= d_ && d <= 31
//    else if m_ in {4, 6, 9, 11}
//        1 <= d_ && d <= 30
//    else
//        1 <= d_ && d <= 29
class month_day
{
    month m_;
    day d_;

    static
    constexpr
    day
    check_invariants(month m, day d)
    {
        return d <= limit[m-1] ? d : throw bad_date{};
    }
public:
    constexpr
    month_day(month m, day d)
        : m_(m),
          d_(check_invariants(m_, d))
        {}

    constexpr
    month_day(month m, int d)
        : m_(m),
          d_(check_invariants(m_, ::day(d)))
        {}

    constexpr
    month_day(int m, day d)
        : m_(::month(m)),
          d_(check_invariants(m_, d))
        {}

    constexpr
    ::month
    month() const
        {return m_;}

    constexpr
    ::day
    day() const
        {return d_;}
};

// invariants:
//    1 <= m_ && m_ <= 12
//    if m_ in {1, 3, 5, 7, 8, 10, 12}
//        1 <= d_ && d <= 31
//    else if m_ in {4, 6, 9, 11}
//        1 <= d_ && d <= 30
//    else if y_.is_leap()
//        1 <= d_ && d <= 29
//    else
//        1 <= d_ && d <= 28
class ymd_date
{
    year y_;
    month m_;
    day d_;

    static
    constexpr
    day
    check_invariants(year y, month m, day d)
    {
        return m != 2 ?
               (
                   d <= limit[m-1] ? d : throw bad_date{}
               ) :
               (
                   y.is_leap() ? (d <= 29 ? d : throw bad_date{}) :
                                 (d <= 28 ? d : throw bad_date{})
               );
    }

    static
    constexpr
    day
    check_invariants(year y, month_day md)
    {
        return md.month() != 2 || md.day() <= 28 || y.is_leap() ?
                   md.day() : throw bad_date{};
    }
public:
    constexpr
    ymd_date(year y, month m, day d)
        : y_(y),
          m_(m),
          d_(check_invariants(y_, m_, d))
        {}

    constexpr
    ymd_date(year y, month m, int d)
        : y_(y),
          m_(m),
          d_(check_invariants(y_, m_, ::day(d)))
        {}

    constexpr
    ymd_date(year y, int m, day d)
        : y_(y),
          m_(m),
          d_(check_invariants(y_, m_, d))
        {}

    constexpr
    ymd_date(int y, month m, day d)
        : y_(y),
          m_(m),
          d_(check_invariants(y_, m_, d))
        {}

    constexpr
    ymd_date(year_month ym, day d)
        : y_(ym.year()),
          m_(ym.month()),
          d_(check_invariants(y_, m_, d))
        {}

    constexpr
    ymd_date(year_month ym, int d)
        : y_(ym.year()),
          m_(ym.month()),
          d_(check_invariants(y_, m_, ::day(d)))
        {}

    constexpr
    ymd_date(year y, month_day md)
        : y_(y),
          m_(md.month()),
          d_(check_invariants(y_, md))
        {}

    constexpr
    ymd_date(int y, month_day md)
        : y_(y),
          m_(md.month()),
          d_(check_invariants(y_, md))
        {}

    constexpr
    ::year
    year() const
        {return y_;}

    constexpr
    ::month
    month() const
        {return m_;}

    constexpr
    ::day
    day() const
        {return d_;}
};

constexpr ymd_date ymd(year(2013), month(5), day(11));

int
main()
{
    return ymd.year();
}
