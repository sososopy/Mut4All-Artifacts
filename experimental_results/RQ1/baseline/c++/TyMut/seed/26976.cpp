
#include <iostream>
#include <unordered_map>
#include <thread>

using namespace std;

void thread_add(unordered_map<int, int>& ht, int from, int to)
{
    for(int i = from; i <= to; ++i)
        ht.insert(unordered_map<int, int>::value_type(i, 0));
}

void main()
{
    unordered_map<int, int> ht;
    thread t[2];

    t[0] = thread(thread_add, ht, 0, 9);
    t[1] = thread(thread_add, ht, 10, 19);

    t[0].join();
    t[1].join();

    std::cout << "size: " << ht.size() << std::endl;
}

