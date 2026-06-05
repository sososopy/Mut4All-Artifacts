struct A {int arr[3] {1, 2, 3};} constexpr a{};

   static_assert( a.arr != a.arr+3 );
