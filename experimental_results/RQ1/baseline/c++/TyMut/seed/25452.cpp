#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> words(6);
    std::vector<std::string> words2;

    std::cout << "Before assignment: size " << words.size() << " capacity " << words.capacity() << std::endl;

    words = words2;

    std::cout << "After assignment: size " << words.size() << " capacity " << words.capacity() << std::endl;

    words[1] = "I should crash here";

    std::cout << "I did not crash. Words contains:" << std::endl;
    for (auto& el : words) {
        std::cout << el << ' ';
    }
}

