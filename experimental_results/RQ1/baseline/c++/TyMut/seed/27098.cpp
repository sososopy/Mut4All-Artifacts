
#include <mutex>
#include <Tracy.hpp>

template<typename T>
class ThreadTask2 {
    TracyLockable(std::mutex, _state_mutex);

public:
    ThreadTask2() {
    }
};

void x() {
    ThreadTask2<int> foo;
}

