
  template< class T, class U = T >
  concept bool
    Assignable( )
  {
    return
      requires( T&& a, U&& b ) {
        //{ forward<T>(a) = forward<U>(b) } -> Same<T&>;        // #1
        //Same<T&, decltype(forward<T>(a) = forward<U>(b))>();  // #2
      };
  }

