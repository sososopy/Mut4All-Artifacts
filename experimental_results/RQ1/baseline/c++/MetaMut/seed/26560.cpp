
#include <array>

template <typename T, std::size_t n>
struct Point
{
 private:
    T value_[n]{};
};

struct Edge
{
    Point<float, 3> start{};
    Point<float, 3> end{};
};

template <typename T, std::size_t n>
class StaticVector
{
 public:
    static StaticVector create()
    {
        StaticVector output;
        return output;
    }

 private:
    std::array<T, n> data{};
};

class Polygon
{
 public:
    using Edges = StaticVector<Edge, 3>;
    Edges edges() const
    {
        auto edges = Edges::create();
        return edges;
    }
};

void foo()
{
    Polygon polygon{};
    auto const edges = polygon.edges();
}
