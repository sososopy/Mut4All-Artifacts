
#include <iostream>
#include <ranges> 
#include <vector>

int main() {
    auto v = std::vector { 1, 2, 3, 4, 5, 6 };
    
    // OK
    for (auto elem : std::ranges::views::reverse(v)) {
        std::cout << elem << ',';
    }
    
    // OK    
    for (auto elem : std::ranges::reverse_view(v)) {
        std::cout << elem << ',';
    }

    // OK
    for (auto elem : v | std::ranges::views::reverse) {
        std::cout << elem << ',';
    }

    // error: missing template arguments before ')' token
    for (auto elem : v | std::ranges::reverse_view) {
        std::cout << elem << ',';
    }
  
    // OK
    for (auto elem : v | std::ranges::views::transform([](auto e) { return e + 1; })) {
        std::cout << elem << ',';
    }    

    // error: class template argument deduction failed:
    for (auto elem : v | std::ranges::transform_view([](auto e) { return e + 1; })) {
        std::cout << elem << ',';
    }
}
