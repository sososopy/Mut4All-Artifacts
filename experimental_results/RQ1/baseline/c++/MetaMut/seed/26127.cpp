
    #include <vector>
    #include <functional>
    #include <utility>
    #include <iterator>
    #include <algorithm>

    constexpr struct adjacent_finder
    {
        template<typename Rng>
        typename Rng::const_iterator
        operator()(Rng const & rng) const
        {
            return std::adjacent_find(rng.begin(), rng.end());
        }
    } adjacent_find {};

    constexpr struct ranger
    {
        template<typename It>
        std::pair<It, It>
        operator()(It begin, It end) const
        {
            return std::make_pair(begin, end);
        }
    } range {};

    constexpr struct prever
    {
        template<typename It>
        It operator()(It begin) const
        {
            return std::prev(begin);
        }
    } prev {};

    constexpr struct ender
    {
        template<typename Rng>
        typename Rng::const_iterator
        operator()(Rng const & rng) const
        {
            return rng.end();
        }
    } end {};

    int main()
    {
        using std::placeholders::_1;

        auto fun = std::bind(
            range,
            std::bind(adjacent_find, _1),
            std::bind(prev, std::bind(end, _1))
        );

        std::vector<int> vi;
        fun(vi);
    }

