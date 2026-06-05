
#include <algorithm>
#include <utility>

template<int K> class List;

template<int K>
class List  {
    template<int KK>
    class Node {
    public:
        static const int SPLIT;
        Node<KK>* next;            // Next pointer
        int keys[KK];       // Set of key/data pairs.

        Node() : next(nullptr) {
            std::fill(keys, keys + KK, 0); 
        }
    };

public:
    std::pair<Node<K>*, Node<K>*> scan();
};

template <int K>
std::pair<List<K>::Node<K>*, List<K>::Node<K>*> List<K>::scan( ) {
    return std::make_pair( nullptr, nullptr );
}

int main() {
    List<1> foo;

    auto bar = foo.scan();
}

