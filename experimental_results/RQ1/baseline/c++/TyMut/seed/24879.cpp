
export  module  hello;

import  "/usr/local/Cellar/gcc/14.1.0/include/c++/14/print";

export namespace hello {
    auto greeting(auto const & x) {
        std::println("Hello, {}!", x);
    }
}

import  hello;

int main() {
    hello::greeting("C++");
}

