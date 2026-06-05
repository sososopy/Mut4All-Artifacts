#include <algorithm>
#include <iostream>
#include <iterator>
#include <thread>

int main() {
    int arr[10];
    std::iota(std::begin(arr), std::end(arr), 1);
    using itr_t = decltype(std::begin(arr));

    // the function that will display each element
    auto f = [] (itr_t first, itr_t last) {while (first != last) std::cout<<*(first++)<<' ';};

    // we have 3 threads so we need to figure out the ranges for each thread to show
    int increment = std::distance(std::begin(arr), std::end(arr)) / 3;
    auto first    = std::begin(arr);
    auto to       = first + increment;
    auto last     = std::end(arr);
    std::thread threads[3] = {
        std::thread{f, first, to},
        std::thread{f, (first = to), (to += increment)},
        std::thread{f, (first = to), last} // go to last here to account for odd array sizes
    };
    for (auto&& t : threads) t.join();
}


