
template< typename T >
using variadic_input = const std::initializer_list<T>;

void function( variadic_input<int> ints )
{}

int main(){
function({1,2});
}

