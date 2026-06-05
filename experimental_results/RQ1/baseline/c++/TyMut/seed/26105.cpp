
#include <limits>
template<class T> void bug() { (std::numeric_limits<int>::max)(); }
int main() { bug<int>(); }



