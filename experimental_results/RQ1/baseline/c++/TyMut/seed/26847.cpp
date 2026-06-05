
template <typename ...TYPES>
   requires(2 == sizeof...(TYPES))
struct MyPair {
   template <typename ...OTHER_TYPES>
      requires sizeof...(TYPES) == sizeof...(OTHER_TYPES)
          and (true and ... and is_constructible<TYPES, OTHER_TYPES>())
   constexpr
   MyPair(OTHER_TYPES&&... args) 
   noexcept((true and ... and is_nothrow_constructible<TYPES, OTHER_TYPES &&>()));
};

