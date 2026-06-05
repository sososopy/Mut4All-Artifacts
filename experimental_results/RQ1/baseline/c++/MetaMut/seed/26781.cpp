
#include <condition_variable>

int main()
{
  std::condition_variable cv;
  cv.notify_all();
  return 0;
}

