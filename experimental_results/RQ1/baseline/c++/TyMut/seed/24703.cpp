
#include <armadillo>
#include <iostream>

template<int param>
class Test {
public:
    void compute() {
        arma::mat matrix = arma::randu(10,10);
        matrix = matrix + matrix.t();
    
        arma::vec eigval;
        arma::mat eigvec;
        arma::eig_sym(eigval, eigvec, matrix);
        arma::mat exp_matrix = eigvec * arma::diagmat(arma::exp(eigval)) * arma::trans(eigvec);

        exp_matrix.print(std::cout);
    }
};

int main() {
    Test<1> test;
    test.compute();
    
    return 0;  
}


