
#include <cstdlib>
#include <cstdio>
#include <thread>
#include <unistd.h>
 
inline void* operator new(size_t size) throw (std::bad_alloc) {
    void* p = malloc(size);
    printf("malloc %zu (%p)\n", size, p);
   
    return p;
}
 
inline void operator delete(void* p) throw() {
    printf("free %p\n", p);
   
    free(p);
}
 
int main()
{
    std::thread t([] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    });
    t.detach();
   
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}

