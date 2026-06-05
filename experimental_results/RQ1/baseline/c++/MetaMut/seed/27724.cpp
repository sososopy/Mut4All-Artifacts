
enum E { e };

constexpr bool arr[1][1] = {{true}};

template<E x, E y>
void check() { 
    static_assert(arr[x][y], ""); 
}

int main() { 
    check<e, e>(); 
}

