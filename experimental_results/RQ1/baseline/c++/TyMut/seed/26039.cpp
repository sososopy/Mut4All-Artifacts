

#include <iostream>
#include <boost/chrono.hpp>
#include <sys/time.h>
#include <math.h>

int main() {
  cpu_set_t  myAffinityMask;
  CPU_ZERO( &myAffinityMask );
  CPU_SET(0, &myAffinityMask );
  sched_setaffinity(0, sizeof(myAffinityMask), &myAffinityMask);

  volatile float* myMemoryA = new float[(1<<24)];
  volatile float* myMemoryB = new float[(1<<24)];

  struct timeval myStart;
  struct timeval myStop;
  struct timeval myResult;

  gettimeofday(&myStart, 0);

  for (size_t i = 0; i < (1<<24); ++i) {
    myMemoryA[i] = i;
    myMemoryB[i] = i+1;
  }
  delete []myMemoryA;
  delete []myMemoryB;

  for (size_t j = 0; j < 100; ++j) {
    volatile float* myMemoryA = new float[(1<<24)];
    volatile float* myMemoryB = new float[(1<<24)];
    for (size_t i = 0; i < (1<<24); ++i) {
      myMemoryA[i] *= sqrtf(myMemoryB[i]);
    }
    delete []myMemoryA;
    delete []myMemoryB;
  }
  gettimeofday(&myStop, 0);

  timersub(&myStop,&myStart,&myResult);

  std::cout << "Time: " <<  myResult.tv_sec*1000 + myResult.tv_usec/1000.0 << std::endl;

  boost::chrono::time_point<boost::chrono::steady_clock> t1 = boost::chrono::high_resolution_clock::now();
  std::cout << "t1: " << t1 << std::endl;
}
