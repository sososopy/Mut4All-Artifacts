
    template< class = void >
    struct S
    {
      friend void foo( S )
      {
        [](){};
      }
    };

