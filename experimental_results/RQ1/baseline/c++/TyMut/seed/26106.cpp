
#include <string>
#include <iostream>
#include <initializer_list>

int main() {
    for ( auto && s : []() -> std::initializer_list< std::string >
          { return { std::string( "first" ), std::string( "second" ) }; }() )
        std::cout << s << '\n';
}

