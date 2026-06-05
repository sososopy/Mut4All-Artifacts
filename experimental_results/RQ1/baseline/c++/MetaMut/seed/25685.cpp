
#include <iostream>

int main() {


    int matrix[][3]{{1,2,3}, {4,5,6}, {7,8,9}};
    auto dyn_matrix = new int[3][3]{{1,2,3}, {4,5,6}, {7,8,9}};

    std::cout << matrix[0][1] << std::endl;
    std::cout << dyn_matrix[0][1] << std::endl;

    return 0;   
}
