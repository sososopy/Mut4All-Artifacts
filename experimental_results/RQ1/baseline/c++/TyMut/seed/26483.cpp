
#include <avr/io.h>
#include <stdint.h>

volatile uint8_t o;

struct FSM {
//    enum class State : uint8_t {A, B, C, D};// <2> 8bit enum
//    enum State {A, B, C, D};                // <3> 8bit enum with -fshort-enums
    enum class State {A, B, C, D};            // <1> 16bit enum with -fshort-enums
    static void f() __attribute__((noinline)) {
        switch(mState) {
        case State::A:
            o = 10;
            break;
        case State::B:
            o = 11;
            break;
        case State::C:
            o = 12;
            break;
        case State::D:
            o = 13;
            break;
        }
    }
private:
    inline static State mState{State::A};
};

int main() {
    while(true) {
        FSM::f();
    }
}
