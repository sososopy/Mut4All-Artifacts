
#ifdef __cplusplus
#include <atomic>
using namespace std;
#else
#include <stdatomic.h>
#endif

atomic<int> i;

int main()
{
  atomic_store(&i, 0);
  return atomic_load(&i);
}
