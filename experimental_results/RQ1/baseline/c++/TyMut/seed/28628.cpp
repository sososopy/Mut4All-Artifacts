
#include <utility>

namespace ns {

    struct S { };

    template<int I>
    constexpr int adl(const S &) {
        return I;
    }

}

namespace redirect {

    template<typename T, int I>
    concept can_call_adl = requires(T &&t) {
        adl<I>(std::forward<T>(t));
    };

    template<int I>
    struct adl_fn {
        template<can_call_adl<I> T>
        constexpr decltype(auto) operator ()(T &&t) const {
            return adl<I>(std::forward<T>(t));
        }
    };

    namespace {

        template<int I>
        constexpr inline adl_fn<I> adl{};

    }

}

static_assert(redirect::can_call_adl<ns::S, 3>);

int main() {
    // return adl<3>(ns::S{});
    return redirect::adl<3>(ns::S{});
}
