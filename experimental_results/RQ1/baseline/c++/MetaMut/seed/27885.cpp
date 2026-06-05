
#include <memory>
class __attribute__((visibility("hidden"))) H {};
class __attribute__((visibility("default"))) V {};
int main()
{
        std::make_shared<H>();
        std::make_shared<V>();
}

