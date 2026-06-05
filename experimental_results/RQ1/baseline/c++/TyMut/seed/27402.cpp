
#include <iostream>
#include <vector>

int main() {
        const std::vector<int> a{1};
        std::vector<int> b;

        std::cout << "before loop: " << b.size() << " < " << a.size() << "\n";

        while(b.size() < a.size()) {
                // uncommenting this fixes the loop with -O2
                //std::cout << ".";
        }
        std::cout << "after loop: " << b.size() << " < " << a.size() << "\n";
}

