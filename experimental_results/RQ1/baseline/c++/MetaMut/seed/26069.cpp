
#include <random>
extern thread_local std::default_random_engine rng;
int main() { return rng(); }
