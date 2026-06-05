
#include <functional>

// Make this non-trivially-copyable so that std::function must put it on the heap
struct lambda {
    lambda() {}
    lambda(const lambda&) {}
    void operator()() const {}
};

// Ternary expression with throw is needed to trigger the issue
std::function<void()> safe_make_function(std::function<void()> f) {
    return f ? f : throw 1;
}

int main() {
    safe_make_function(lambda{});
}
