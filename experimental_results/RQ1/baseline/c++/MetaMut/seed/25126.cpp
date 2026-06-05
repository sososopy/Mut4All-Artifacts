

namespace std {
    template <bool, class> struct enable_if;
}

template <class T>
struct BidirectionalRange;

template<typename Rng>
struct stride_view
{
    struct adaptor
    {
        template<int _c_11 = 42, typename std::enable_if<
            _c_11 == 43 || BidirectionalRange<Rng>(), int>::type = 0>
        void clean() const
        {}
        template<int _c_14 = 42, typename std::enable_if<
            _c_14 == 43 || !BidirectionalRange<Rng>(), int>::type = 0>
        void clean() const
        {}
        void advance()
        {
            clean();
        }
    };
};

