
#include <atomic>

class A {
    struct State {
        int flag;
        std::atomic<State*> next_[];
    };
    void state(int f) {
        State s;
        s.flag = f;
    }
    State state_;
};

