
#include <pthread.h>
class Test {
    pthread_mutex_t m;
    Test();
};

Test::Test():m(PTHREAD_MUTEX_INITIALIZER) {}

