
    #include <array>
    #include <cassert>
    
    namespace /* anonymous */
    {
    
      constexpr auto
      make_array(const int val) noexcept
      {
        std::array<int, 2> result = { { val, 0 } };
        return result;
      }
    
      // Replacing `constexpr` by `const` doesn't change anything.
      constexpr auto numbers_static = make_array(42);
    
    }
    
    int
    main()
    {
      const auto numbers_automatic = make_array(42);
      assert(numbers_automatic[0] == 42);  // okay
      assert(numbers_static[0] == 42);     // fails
    }

