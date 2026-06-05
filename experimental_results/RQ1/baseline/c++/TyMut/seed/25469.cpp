
#include <vector>

template <typename Left, typename Op, typename Right>
struct Expression {
    Expression(const Left & l, const Right & r) : left(l), right(r) {}
    float operator[](unsigned int index) const {
        return Op::apply(left[index], right[index]);
    }
    const Left & left;
    const Right & right;
};

struct plus {
    static float apply(float l, float r) { return l + r; }
};

template <typename Left, typename Right>
Expression<Left, plus, Right> operator+(const Left & l, const Right & r)
{
    return Expression<Left, plus, Right>(l, r);
}

int main()
{
    std::vector<float> v1{2, 3.4, 5};
    std::vector<float> v2{3, 5.0, 4};
    std::vector<float> v3{4, 6.0, 1};
    auto v4 = v1 + v2 + v3;
    float total = 0;
    for (auto i = 0u; i != v1.size(); ++i)
        total += v4[i];
    return total;
}
