
#include <sstream>
#include <string>

template <class T>
concept StreamCanReceiveString = requires(T& t, std::string s) {
    { t << s };
};

struct NotAStream {};
struct UnrelatedType {};

template <StreamCanReceiveString S>
S& operator<<(S& s, UnrelatedType) {
    return s;
}

static_assert(!StreamCanReceiveString<NotAStream>);

static_assert(StreamCanReceiveString<std::stringstream>);
