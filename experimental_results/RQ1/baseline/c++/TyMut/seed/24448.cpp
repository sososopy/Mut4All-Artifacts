
namespace N {
    class I {};
}

template <typename, typename>
class J {};

class S {
    J<int, N::I> j = J<int, N::I>{};
};

int main() {}
