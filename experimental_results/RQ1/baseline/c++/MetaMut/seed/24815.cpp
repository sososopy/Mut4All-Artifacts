
#include <cassert>
#include <vector>


// Since K+1 the returned vector has size() >= 1
inline auto f(size_t N, size_t K) {
    return std::vector<std::vector<size_t>>(K+1, std::vector<size_t>(N, 0));
}

// create another vector, but derive size from the previous vector
inline auto g(std::vector<std::vector<size_t>> const& v1) {
    size_t const K = v1.size() - 1;
    size_t const N = v1[0].size();
    return std::vector<std::vector<size_t>>(K+1, std::vector<size_t>(N, 0));
}

struct S {
    std::vector<size_t> v1;
    std::vector<size_t> v2;
    // v1, v2always have the same length
};

auto h(size_t N, size_t K) {
    assert(N>0);
    assert(N >= K);

    auto v1 = f(N, K);
    auto v2 = g(v1);

    auto ss = std::vector<S>{};
    for (size_t i{0}; i < v1.size(); ++i) {
        ss.emplace_back(S{v1[i], {}});
    }

    for (auto& s : ss) {
        s.v1.back() += 1;
    }

    return ss;
}

auto all = h(4, 3);
