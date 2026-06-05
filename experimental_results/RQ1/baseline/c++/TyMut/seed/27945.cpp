#include <array>
#include <iostream>

using Coordinates = std::array<double, 3>;

Coordinates map( const Coordinates& c, size_t level )
{
    Coordinates result{ c[1], c[2], c[0] };
    
    if( level != 0 )
    {
      result =  map( result, level - 1 );
    }
    
    return result;
}

int main( )
{
    Coordinates vecOfCoordinates = { 1.0, 2.0, 3.0 };
    size_t level = 1;
    
    auto result = map( vecOfCoordinates, level );
    
    std::cout << "result = (" << result[0] << ", " << result[1] << ", " << result[2] << "), should be (3, 1, 2)" << std::endl;
    
    return 0;
    
}
