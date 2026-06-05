
#include <future>

int main()
{
        std::promise<int> promise;
        (void) promise.get_future();
}

