
#include <iostream>
#include <functional>
#include <random>
#include <chrono>

void print_random(std::function<double()>& dice) {
	std::cout << dice() << ", ";
} 

int main(int argc, char **argv) {
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::normal_distribution<double> distribution(0,6);
	std::function<double()> dice = std::bind (distribution, generator);
	std::cout << "Random numbers: ";
	for(int i=0; i<30; ++i) print_random(dice);
	std::cout << std::endl;
	return 0;
}

