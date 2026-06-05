
#include <string>

typedef std::string Container;

template <typename T>
class Base {
    public:
        Container container;

        char Sum() const {
            auto Summarizer = [this] () -> char {
                for (auto& c : container) { // segfaults here
                    return c;
                }
            };

            return Summarizer();
        }
};

int main() {
    return 0;
}
