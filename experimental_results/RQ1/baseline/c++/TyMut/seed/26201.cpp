

#include <algorithm>

class Bug {
    static constexpr unsigned max = 2;
public:
    bool bug(int pos) { return pos < std::min(3u, max); }
};

int main() {
    return Bug().bug(0);
}

