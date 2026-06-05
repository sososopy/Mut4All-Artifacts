

#include <iostream>
#include <array>
#include <algorithm>
#include <atomic>

int main(int argc, char* argv[]) {

    // std::array<std::atomic<int>, 1000000>  arr;         // default initialization (i.e., random data) = FAST

    std::array<std::atomic<int>, 1000000>  arr{{}};   // zero initialization = FOREVER

    //std::array<std::atomic<int>, 1000000>  arr{};   // zero initialization = FOREVER

    //std::array<std::atomic<int>, 1000000>  arr={{}};     // zero init via assignment = FOREVER

    //std::array<int, 1000000>  arr={{}};     // zero init non-atomic = FAST

    std::cerr << "sum = " << std::accumulate(arr.begin(), arr.end(), 0) << std::endl;
}
