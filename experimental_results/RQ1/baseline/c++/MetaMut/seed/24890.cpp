
#include <array>     // std::array
#include <bit>       // std::countr_zero
#include <optional>  // std::optional
#include <utility>   // std::pair

template <int U>
struct VEB {
   private:
    // doesn't seem to ICE without a pair
    static constexpr std::pair<int, int> G = {1, 1};
    // doesn't seem to ICE without an array, with at least one of the template params being one of the pair.
    std::array<VEB<G.first>, 1> cluster = {};
    // std::array<VEB<1>, G.second> cluster = {};
   public:
    constexpr void insert(int x) {
        cluster[x].min();
    }
};

template <int U>
    requires(U <= 64)
struct VEB<U> {
   private:
    unsigned cluster = {};
   public:
    // doesn't seem to ICE without an optional
    constexpr std::optional<int> min() const {
        // doesn't seem to ICE without std::countr_zero
        return std::countr_zero(cluster);
    }
};

// doesn't ICE without constant folding. either constexpr or consteval
constexpr auto check_correctness() {
    VEB<65> veb{};
    veb.insert(0);
    return true; // doesn't ICE without the return
};

// alternatively, if consteval, ICEs without return
// consteval auto check_correctness() {
//     VEB<65> veb{};
//     veb.insert(0);
// } // does not need the return to ICE

// doesn't ICE without constant folding, -O or better is required
int main() {
    check_correctness();
}

