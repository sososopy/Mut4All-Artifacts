
#include <vector>
#include <memory>

int main() {
        std::vector<std::unique_ptr<int>> v;
        std::unique_ptr<int> px(new int (1));
        v.push_back(px);
}
