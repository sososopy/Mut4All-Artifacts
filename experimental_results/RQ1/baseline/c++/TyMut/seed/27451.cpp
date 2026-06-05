
#include <coroutine>

using namespace std;

struct generator {
    struct promise_type {
        generator get_return_object();
        void return_void();
        void unhandled_exception();
        suspend_always initial_suspend();
        suspend_always final_suspend();

        template<typename Arg>
        suspend_always yield_value(Arg&&) {
            return {};
        }
    };
};


generator x() {
    co_yield "foo";
}
