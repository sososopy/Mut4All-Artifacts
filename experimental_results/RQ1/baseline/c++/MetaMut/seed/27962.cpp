
#include <algorithm>
#include <optional>
#include <string>

void f(const std::string& t)
{    
    if (std::any_of(t.begin(), t.end(), [](const auto& c) {
                return c == 0b01000000;
            })) {
        std::string s;
        
        std::optional<std::string::const_iterator> f;
        for (auto i = t.begin(); i != t.end(); ++i) {
            const auto n = *i;
            if (n == 0b01000000) {
                f = i;
            } else if (f) {
                std::string_view key(&(**f), 2);
                s += key;
                f.reset();
            }
        }
    }
}
