
    template< typename T >
    class A
    {
    };
    
    template<>
    class A< void >
    {
            template< typename X >
            friend class A;
    };

