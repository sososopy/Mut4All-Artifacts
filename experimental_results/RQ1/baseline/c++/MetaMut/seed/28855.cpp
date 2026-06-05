
    struct plus {
      template<typename... Args>
      using invoke = void;
    };
    
    template <typename Fn, typename... Args>
    using invoke = typename Fn::template invoke<Args...>;
    
    template <typename Fn>
    struct compose
    {
      template <typename X, typename Y>
      using F = invoke<Fn, X, Y>;
    
      template <typename X>
      using invoke = invoke<Fn, X, X>;
    };
    
    using Q = compose<plus>::F<int, int>;
