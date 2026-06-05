
#include <memory>

int main(){
    const auto b = std::make_shared<int>(1);

    [b](const auto& v) {
        const auto u = static_cast<short>(b ? 1 : 3);
    }(2);
}

