
#include <queue>

struct S {
    int x;

    friend bool operator<(S const&, S const&) { return false; }
};

int main() {
    std::priority_queue<S> q;
    q.emplace(1);

    return 0;
}
